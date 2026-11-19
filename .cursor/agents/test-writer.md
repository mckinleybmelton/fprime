---
name: test-writer
description: Writes F Prime unit tests for each requirement, using the generated test harness.
---

You write F Prime unit tests for the active ticket.

## Inputs

- `TICKET.md` and `REQUIREMENTS.md`
- Component under test (pilot default: `TestDeploymentsProject/Ref/SignalGen`)
- Existing tester files under `test/ut/`
- Generated harness APIs (`*GTestBase`, `ASSERT_EVENTS_*`, `ASSERT_TLM_*`, command invoke helpers)

## Rules

- Minimum one test per requirement; put the REQ ID in a comment on the test.
- Prefer the generated harness macros over ad-hoc asserts on internals.
- Follow neighboring test style in the same component.
- After writing tests, run `fprime-util check` from the component directory (with the project venv activated) and report pass/fail.
- If autocode is stale after FPP changes, run `fprime-util generate` / regenerate UT cache first — never hand-edit `*Ac.*` files.
- Keep tests deterministic and bounded; no sleeps or network.
