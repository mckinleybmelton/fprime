---
name: add-command-telemetry
description: Add a commandable telemetry/event feature to an F Prime component using the model-first TDD loop with requirement traceability.
---

# Add command / telemetry / event (F Prime)

Reusable workflow captured from the NASA/F Prime governed lifecycle pilot.

## Inputs

- Component path (e.g. `TestDeploymentsProject/Ref/SignalGen`)
- Requirement IDs (from `REQUIREMENTS.md` or the ticket)
- Desired command (or parameter), telemetry channel(s), and event(s)

## Steps

1. **Read context:** component `.fpp` / `.fppi`, SDD if present, existing `test/ut/` tests, and neighboring style.
2. **Plan:** list every file to change, one test per REQ ID, and the verify command. Get human approval before coding when in Plan mode.
3. **Model first:** declare command/param, telemetry, and events in FPP — never only in C++.
4. **Generate:** run `fprime-util generate` (and UT generate if needed). Do not edit `*Ac.hpp` / `*Ac.cpp`.
5. **Failing tests:** add unit tests that cite each REQ ID and use `ASSERT_EVENTS_*` / `ASSERT_TLM_*`.
6. **Implement:** handlers and logic until behavior matches the model and tests.
7. **Verify:** `fprime-util check` (add `--coverage` when available). No green, no PR.
8. **Trace:** produce a requirement-to-code-to-test matrix in the PR description (use the `traceability-auditor` agent).
9. **Capture:** if the session improved this skill, update this file.

## Checks / definition of done

- [ ] FPP is the source of truth for the new interface
- [ ] No autocoded files hand-edited
- [ ] Each REQ ID has a named unit test
- [ ] `fprime-util check` passes
- [ ] Trace matrix present in the PR
- [ ] Humans approved the plan and the merge

## Anti-patterns

- Patching generated headers "just this once"
- Implementing C++ commands without FPP declarations
- Shipping without tests that name requirement IDs
