import { defineTool } from "@grok/agent-sdk/tools";
import * as z from "zod/v4";

import {
  COMMENT_MARKER,
  commentBody,
  pullRequestFromSession,
  readReviewRecord,
  reviewKey,
} from "../lib/pr.js";

export default defineTool({
  description:
    "Create or update the single pull-request comment for this review. Call once per turn. Never approves the pull request.",
  effect: "idempotent",
  inputSchema: z.object({
    verdict: z.enum(["request_changes", "comment"]),
    body: z.string().min(1).describe("Markdown matrix and the unit-test check note."),
  }),
  async execute({ verdict, body }, ctx) {
    if (ctx.session.purpose === "eval") {
      return { posted: false, reason: "eval", verdict };
    }

    const ref = pullRequestFromSession(ctx.session.continuationKey);
    if (ref === undefined) {
      return { posted: false, reason: "no_pull_request", verdict };
    }

    const text = commentBody(verdict, body);
    const octokit = await ctx.host.github.getOctokit();
    const stored = readReviewRecord(await ctx.host.kv.get(reviewKey(ref)));

    let commentId = stored?.commentId ?? null;
    if (commentId === null) {
      for (let page = 1; page <= 5; page += 1) {
        const { data: comments } = await octokit.rest.issues.listComments({
          owner: ref.owner,
          repo: ref.repo,
          issue_number: ref.number,
          per_page: 100,
          page,
        });
        for (const comment of comments) {
          if (comment.body?.includes(COMMENT_MARKER)) {
            commentId = comment.id;
          }
        }
        if (comments.length < 100) {
          break;
        }
      }
    }

    if (commentId !== null) {
      try {
        await octokit.rest.issues.updateComment({
          owner: ref.owner,
          repo: ref.repo,
          comment_id: commentId,
          body: text,
        });
      } catch {
        commentId = null;
      }
    }
    if (commentId === null) {
      const { data: created } = await octokit.rest.issues.createComment({
        owner: ref.owner,
        repo: ref.repo,
        issue_number: ref.number,
        body: text,
      });
      commentId = created.id;
    }

    const { data: pull } = await octokit.rest.pulls.get({
      owner: ref.owner,
      repo: ref.repo,
      pull_number: ref.number,
    });
    await ctx.host.kv.put(reviewKey(ref), {
      headSha: pull.head.sha,
      commentId,
    });

    return {
      posted: true,
      verdict,
      commentId,
      pr: `${ref.owner}/${ref.repo}#${ref.number}`,
    };
  },
});
