import { readFile } from "node:fs/promises";
import path from "node:path";

import { parseGitHubPrContinuationKey } from "@grok/agent-sdk/channels/github";

/** GitHub Actions job name. The check run on the pull request uses this name. */
export const UNIT_TEST_CHECK_NAME = "F Prime unit tests";

/** Hidden marker so a later turn updates the same comment. */
export const COMMENT_MARKER = "<!-- fprime-pr-quality -->";

export const EVIDENCE_FILE = "pr-evidence.json";

const PATCH_CAP = 3000;
const TEXT_CAP = 12000;

export type PullRequestRef = {
  owner: string;
  repo: string;
  number: number;
};

export type ChangedFile = {
  path: string;
  status?: string;
  patch?: string;
};

export type UnitTestCheck = {
  state: string;
  name: string;
  excerpt?: string;
  partial?: boolean;
};

export type EvidenceFile = {
  title?: string;
  headSha?: string;
  baseRef?: string;
  requirements?: string;
  ticket?: string;
  files?: ChangedFile[];
  unitTestCheck?: UnitTestCheck;
};

export type ReviewRecord = {
  headSha: string;
  commentId: number | null;
};

export function reviewKey(ref: PullRequestRef): string {
  return `pr-quality-review:${ref.owner}/${ref.repo}#${ref.number}`;
}

export function pullRequestFromSession(continuationKey: string | null | undefined): PullRequestRef | undefined {
  if (continuationKey === null || continuationKey === undefined || continuationKey === "") {
    return undefined;
  }
  const parsed = parseGitHubPrContinuationKey(continuationKey);
  if (parsed === undefined) {
    return undefined;
  }
  return { owner: parsed.owner, repo: parsed.repo, number: parsed.number };
}

export function isJsonRecord(value: unknown): value is Record<string, unknown> {
  return typeof value === "object" && value !== null && !Array.isArray(value);
}

export function readReviewRecord(value: unknown): ReviewRecord | undefined {
  if (!isJsonRecord(value) || typeof value.headSha !== "string") {
    return undefined;
  }
  const commentId = value.commentId;
  if (commentId !== null && commentId !== undefined && typeof commentId !== "number") {
    return undefined;
  }
  return { headSha: value.headSha, commentId: typeof commentId === "number" ? commentId : null };
}

export async function readEvidenceFile(workspaceDir: string): Promise<EvidenceFile> {
  const raw = await readFile(path.join(workspaceDir, EVIDENCE_FILE), "utf8");
  const parsed: unknown = JSON.parse(raw);
  if (typeof parsed !== "object" || parsed === null || Array.isArray(parsed)) {
    throw new Error(`${EVIDENCE_FILE} must be a JSON object`);
  }
  return parsed as EvidenceFile;
}

export function capText(value: string | undefined, limit = TEXT_CAP): string | undefined {
  if (value === undefined) {
    return undefined;
  }
  if (value.length <= limit) {
    return value;
  }
  return `${value.slice(0, limit)}\n…[truncated]`;
}

export function capPatch(value: string | undefined): string | undefined {
  return capText(value, PATCH_CAP);
}

export function commentBody(verdict: "request_changes" | "comment", body: string): string {
  const headline = verdict === "request_changes" ? "**Request changes.**" : "**Comment.**";
  const trimmed = body.trim();
  const withoutMarker = trimmed.startsWith(COMMENT_MARKER)
    ? trimmed.slice(COMMENT_MARKER.length).trim()
    : trimmed;
  return `${COMMENT_MARKER}\n${headline}\n\n${withoutMarker}`;
}

export function isUnitTestCheck(name: string | undefined): boolean {
  if (name === undefined) {
    return false;
  }
  return name === UNIT_TEST_CHECK_NAME || name.endsWith(`/ ${UNIT_TEST_CHECK_NAME}`);
}

export function excerptLooksPartial(excerpt: string | undefined): boolean {
  if (excerpt === undefined) {
    return false;
  }
  return excerpt.includes("Not run (cap)") || excerpt.includes("not run");
}
