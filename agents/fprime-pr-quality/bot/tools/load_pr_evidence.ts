import { defineTool } from "@grok/agent-sdk/tools";
import * as z from "zod/v4";

import {
  capPatch,
  capText,
  pullRequestFromSession,
  readEvidenceFile,
  readReviewRecord,
  reviewKey,
  type ChangedFile,
} from "../lib/pr.js";

function decodeContent(content: string, encoding: string | undefined): string {
  if (encoding === "base64") {
    return Buffer.from(content, "base64").toString("utf8");
  }
  return content;
}

export default defineTool({
  description:
    "Read the pull request pinned to this session: REQUIREMENTS.md, TICKET.md, and the changed files. The repository comes from the session, not from the arguments.",
  effect: "read",
  inputSchema: z.object({}),
  async execute(_input, ctx) {
    if (ctx.session.purpose === "eval") {
      const evidence = await readEvidenceFile(ctx.workspaceDir);
      return {
        source: "fixture",
        title: evidence.title ?? "",
        headSha: evidence.headSha ?? "",
        baseRef: evidence.baseRef ?? "",
        requirements: capText(evidence.requirements) ?? "",
        ticket: capText(evidence.ticket) ?? "",
        files: (evidence.files ?? []).map((file) => ({
          path: file.path,
          status: file.status,
          patch: capPatch(file.patch),
        })),
      };
    }

    const ref = pullRequestFromSession(ctx.session.continuationKey);
    if (ref === undefined) {
      return {
        error: "No pull request is pinned to this session.",
      };
    }

    const pullRequest = ref;
    const octokit = await ctx.host.github.getOctokit();
    const { data: pull } = await octokit.rest.pulls.get({
      owner: pullRequest.owner,
      repo: pullRequest.repo,
      pull_number: pullRequest.number,
    });
    const headSha = pull.head.sha;
    const { data: changed } = await octokit.rest.pulls.listFiles({
      owner: pullRequest.owner,
      repo: pullRequest.repo,
      pull_number: pullRequest.number,
      per_page: 100,
    });

    async function readRepoFile(filePath: string): Promise<string> {
      try {
        const { data } = await octokit.rest.repos.getContent({
          owner: pullRequest.owner,
          repo: pullRequest.repo,
          path: filePath,
          ref: headSha,
        });
        if (Array.isArray(data) || data.type !== "file" || data.content === undefined) {
          return "";
        }
        return capText(decodeContent(data.content, data.encoding)) ?? "";
      } catch {
        return "";
      }
    }

    const files: ChangedFile[] = changed.map((file) => ({
      path: file.filename,
      status: file.status,
      patch: capPatch(file.patch ?? undefined),
    }));

    const existing = readReviewRecord(await ctx.host.kv.get(reviewKey(pullRequest)));
    await ctx.host.kv.put(reviewKey(pullRequest), {
      headSha,
      commentId: existing?.commentId ?? null,
    });

    return {
      source: "github",
      owner: pullRequest.owner,
      repo: pullRequest.repo,
      number: pullRequest.number,
      title: pull.title,
      headSha: pull.head.sha,
      baseRef: pull.base.ref,
      draft: pull.draft ?? false,
      requirements: await readRepoFile("REQUIREMENTS.md"),
      ticket: await readRepoFile("TICKET.md"),
      files,
      truncated: changed.length >= 100,
    };
  },
});
