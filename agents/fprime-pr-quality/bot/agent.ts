import { defineAgent } from "@grok/agent-sdk";

export default defineAgent({
  description:
    "Reviews an F Prime pull request for requirement traceability and unit-test coverage, then posts one comment.",
  model: {
    id: "grok-4.7",
    params: [
      { id: "reasoning_effort", value: "high" },
      { id: "fast", value: "true" },
    ],
  },
});
