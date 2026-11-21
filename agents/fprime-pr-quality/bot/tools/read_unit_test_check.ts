import { defineTool } from "@grok/agent-sdk/tools";
import * as z from "zod/v4";

import {
  capText,
  excerptLooksPartial,
  isUnitTestCheck,
  pullRequestFromSession,
  readEvidenceFile,
  UNIT_TEST_CHECK_NAME,
} from "../lib/pr.js";

export default defineTool({
  description:
    "Read the GitHub check named \"F Prime unit tests\" for this session's pull request head. Pending and missing are normal results. The repository comes from the session.",
  effect: "read",
  inputSchema: z.object({}),
  async execute(_input, ctx) {
    if (ctx.session.purpose === "eval") {
      const evidence = await readEvidenceFile(ctx.workspaceDir);
      const check = evidence.unitTestCheck;
      if (check === undefined) {
        return { name: UNIT_TEST_CHECK_NAME, state: "missing", partial: false };
      }
      return {
        name: check.name ?? UNIT_TEST_CHECK_NAME,
        state: check.state,
        excerpt: capText(check.excerpt, 1500),
        partial: check.partial === true || excerptLooksPartial(check.excerpt),
      };
    }

    const ref = pullRequestFromSession(ctx.session.continuationKey);
    if (ref === undefined) {
      return { error: "No pull request is pinned to this session." };
    }

    const octokit = await ctx.host.github.getOctokit();
    const { data: pull } = await octokit.rest.pulls.get({
      owner: ref.owner,
      repo: ref.repo,
      pull_number: ref.number,
    });
    const { data } = await octokit.rest.checks.listForRef({
      owner: ref.owner,
      repo: ref.repo,
      ref: pull.head.sha,
      per_page: 100,
    });
    const runs = data.check_runs.filter((run) => isUnitTestCheck(run.name));
    if (runs.length === 0) {
      return {
        name: UNIT_TEST_CHECK_NAME,
        state: "missing",
        headSha: pull.head.sha,
        partial: false,
      };
    }

    const latest = runs.reduce((best, run) => {
      const bestAt = best.started_at ?? "";
      const runAt = run.started_at ?? "";
      return runAt >= bestAt ? run : best;
    });

    if (latest.status !== "completed") {
      return {
        name: latest.name,
        state: "pending",
        status: latest.status,
        headSha: pull.head.sha,
        partial: false,
      };
    }

    let excerpt = capText(latest.output.summary ?? latest.output.title ?? undefined, 1500);
    try {
      const { data: notes } = await octokit.rest.checks.listAnnotations({
        owner: ref.owner,
        repo: ref.repo,
        check_run_id: latest.id,
        per_page: 20,
      });
      const lines = notes.map((note) => `${note.annotation_level}: ${note.message}`);
      if (lines.length > 0) {
        excerpt = capText([excerpt, ...lines].filter((line) => line !== undefined).join("\n"), 1500);
      }
    } catch {
      // The check conclusion is still useful when annotations are unreadable.
    }

    return {
      name: latest.name,
      state: latest.conclusion ?? "missing",
      headSha: pull.head.sha,
      excerpt,
      partial: excerptLooksPartial(excerpt),
    };
  },
});
