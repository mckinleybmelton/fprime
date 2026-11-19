# Ticket: SignalGen commandable high-threshold check

**Component:** `Ref::SignalGen`  
**Location:** `TestDeploymentsProject/Ref/SignalGen/`  
**Traceability:** REQ-HM-001, REQ-HM-002, REQ-HM-003 in `REQUIREMENTS.md`

## Summary

Add a commandable high-threshold check to `Ref::SignalGen`. Operators must be able to set a high threshold for the generated signal output. When the current `Output` value exceeds that threshold, the component emits a warning event. Telemetry must report the active threshold so ground can verify the configured limit.

## Acceptance criteria

1. **REQ-HM-001** — A command (preferred) or parameter sets a high threshold for signal `Output`.
2. **REQ-HM-002** — A telemetry channel reports the active high threshold.
3. **REQ-HM-003** — A warning-severity event fires when `Output` exceeds the configured high threshold.
4. Unit tests cover each requirement ID (name the REQ ID in a comment).
5. `fprime-util check` (and coverage if available) passes for `SignalGen`.
6. PR description includes a requirement-to-code-to-test matrix.

## Process constraints (pilot workflow)

- Model first: change FPP (commands / telemetry / events), then `fprime-util generate`, then implement.
- Never edit autocoded files (`*Ac.hpp`, `*Ac.cpp`, anything under `build-fprime-*`).
- TDD: write failing unit tests, then implement until green.
- Cite REQ IDs in tests and the PR description.

## Notes for the agent

- Existing telemetry already includes `Output: F32` (current Y value). Reuse that as the monitored value.
- Follow neighboring FPP style in `Commands.fppi`, `Telemetry.fppi`, and `Events.fppi`.
- Use generated harness macros (`ASSERT_EVENTS_*`, `ASSERT_TLM_*`) in unit tests.
