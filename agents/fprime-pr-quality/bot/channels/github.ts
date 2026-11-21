import { defaultGitHubAuth, githubChannel } from "@grok/agent-sdk/channels/github";

import { isUnitTestCheck, UNIT_TEST_CHECK_NAME } from "../lib/pr.js";

const REVIEW_ACTIONS = new Set(["opened", "reopened", "ready_for_review", "synchronize"]);
const WAKE_WINDOW_MS = 15_000;

const REVIEW_MESSAGE = [
  "Review this pull request.",
  "Call load_pr_evidence and read_unit_test_check.",
  "Delegate to traceability-auditor and test-writer.",
  "Call submit_quality_review once.",
  "Do not approve the pull request and do not push commits.",
].join(" ");

function wakeKey(owner: string, repo: string, number: number): string {
  return `pr-quality-wake:${owner}/${repo}#${number}`;
}

export default githubChannel({
  botName: "fprime-pr-quality",
  // Repositories come from `serve --repo` and `deploy --cursor-events-repo`.
  // commitStatus opts the Cursor GitHub lease up so the Checks API can post.
  // Tools in this agent never push.
  cursorAccount: true,
  deliverReplies: false,
  progress: {
    requireSuccessfulTools: ["submit_quality_review"],
    commitStatus: {
      context: "F Prime PR quality",
      pending: "Reviewing requirements, tests, and the unit-test check.",
      success: "Review posted.",
      failure: "Review did not finish.",
    },
    banner: {
      pending: "Reviewing requirements, tests, and the unit-test check.",
      success: "Review posted.",
      failure: "Review did not finish.",
    },
  },
  onPullRequest: async (ctx, pr) => {
    if (pr.draft || !REVIEW_ACTIONS.has(pr.action)) {
      return null;
    }
    const key = wakeKey(ctx.repository.owner, ctx.repository.name, pr.number);
    const existing = await ctx.host.kv.get(key);
    const now = Date.now();
    if (
      typeof existing === "object" &&
      existing !== null &&
      !Array.isArray(existing) &&
      existing.headSha === pr.headSha &&
      typeof existing.at === "number" &&
      now - existing.at < WAKE_WINDOW_MS
    ) {
      return null;
    }
    await ctx.host.kv.put(key, { headSha: pr.headSha, at: now });
    return {
      auth: defaultGitHubAuth(ctx),
      title: `Review ${ctx.repository.fullName}#${pr.number}`,
      message: `${REVIEW_MESSAGE} Head ${pr.headSha}. ${pr.url}`,
    };
  },
  onCheckRun: async (ctx, check) => {
    if (!isUnitTestCheck(check.name) || check.action !== "completed") {
      return null;
    }
    const linked = check.pullRequests[0];
    if (linked === undefined) {
      return null;
    }
    const octokit = await ctx.github.getOctokit();
    const { data: pull } = await octokit.rest.pulls.get({
      owner: ctx.repository.owner,
      repo: ctx.repository.name,
      pull_number: linked.number,
    });
    if (pull.draft) {
      return null;
    }
    const seenKey = `pr-quality-check:${ctx.repository.fullName}#${linked.number}:${check.checkRunId}:${check.conclusion ?? "none"}`;
    if ((await ctx.host.kv.get(seenKey)) !== undefined) {
      return null;
    }
    await ctx.host.kv.put(seenKey, { at: Date.now() });
    return {
      auth: defaultGitHubAuth(ctx),
      title: `Review ${ctx.repository.fullName}#${linked.number}`,
      message: `${REVIEW_MESSAGE} The unit-test check "${UNIT_TEST_CHECK_NAME}" completed with conclusion ${check.conclusion ?? "unknown"} for head ${check.headSha}. Update the same review.`,
    };
  },
});
