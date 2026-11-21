import { defineAgent } from "@grok/agent-sdk";

export default defineAgent({
  description:
    "Reviews F Prime unit tests against requirements. Read-only. Reports missing REQ citations and what the unit-test check means. Does not edit files or run fprime-util.",
});
