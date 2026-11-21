import { readFileSync } from "node:fs";

import { defineEval } from "@grok/agent-sdk/evals";

const fixture = readFileSync(new URL("./fixtures/missing-req-hm-003.json", import.meta.url), "utf8");

function outputWasNotPosted(value: unknown): boolean {
  if (typeof value !== "object" || value === null || !("posted" in value)) {
    return false;
  }
  return value.posted === false;
}

export default defineEval({
  tags: ["smoke"],
  description: "A green unit-test check still requests changes when REQ-HM-003 has no citing test.",
  async test(t) {
    await t.send(
      "Review this pull request. Follow your instructions. The evidence is in pr-evidence.json, which your tools read. There is no GitHub URL.",
      { workspaceFiles: { "pr-evidence.json": fixture } },
    );
    t.succeeded();
    t.calledTool("load_pr_evidence");
    t.calledTool("read_unit_test_check");
    t.calledSubagent("traceability-auditor");
    t.calledSubagent("test-writer");
    t.calledTool("submit_quality_review", {
      count: 1,
      input: {
        verdict: "request_changes",
        body: (body: unknown) =>
          typeof body === "string" && /REQ-HM-003/.test(body) && /missing/i.test(body),
      },
      output: outputWasNotPosted,
    });
  },
});
