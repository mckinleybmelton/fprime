import { defineAgent } from "@grok/agent-sdk";

export default defineAgent({
  description:
    "Builds a requirement-to-code-to-test matrix for the pull request. Read-only. Returns markdown. Does not edit files or post to GitHub.",
});
