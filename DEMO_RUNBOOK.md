# SignalGen demo runbook (~17 minutes + framing)

Use with `DEMO_SCRIPTS.md` (five Option A rehearsals) and `DEMO_PROMPTS.md`.

## What is already merged

| Artifact | Where | What it proves |
| --- | --- | --- |
| Setup ([#1](https://github.com/mckinleybmelton/fprime/pull/1), merged) | `devel` | Team inherits the governed workflow by cloning |
| Ticket dry-run ([#2](https://github.com/mckinleybmelton/fprime/pull/2)) | `cursor/signalgen-dry-run-f241` | One ticket goes to a tested, traced change (bots + matrix) |

The SignalGen feature is not on `devel`. Live Demo 1 starts there. PR #2 is the receipt you show if the build stalls.

## Before the room

1. Privacy Mode on; mention it in framing. Sign into the Atlassian connection. Practice tickets on the NASA board: [NASA-1](https://mckinley-melton.atlassian.net/browse/NASA-1) live SignalGen, [NASA-2](https://mckinley-melton.atlassian.net/browse/NASA-2) BlockDriver, [NASA-3](https://mckinley-melton.atlassian.net/browse/NASA-3) PingReceiver, [NASA-4](https://mckinley-melton.atlassian.net/browse/NASA-4) SendBuffApp stop-at-plan, [NASA-5](https://mckinley-melton.atlassian.net/browse/NASA-5) interrupt rehearsal. A rehearsal moves only the key you pull. NASA-5 must not move NASA-1. Stay in one Plan-mode chat. Do not switch modes.
2. Start the **live** demo from `devel` (workflow only — no SignalGen feature yet):

   ```bash
   git fetch origin
   git checkout devel
   git pull origin devel
   source fprime-venv/bin/activate
   ```

3. If a step stalls live, jump ahead with tags (on the dry-run branch history):

   | Tag | Contents |
   | --- | --- |
   | `after-plan` | Setup branch tip (workflow + ticket/reqs) |
   | `after-build` | FPP + implementation on dry-run branch |
   | `after-tests` | Full dry-run green (4 unit tests) |

   ```bash
   git checkout after-tests -- TestDeploymentsProject/Ref/SignalGen
   ```

4. Pre-run PR for Bugbot / Security Review: https://github.com/mckinleybmelton/fprime/pull/2 (ticket dry-run), not setup https://github.com/mckinleybmelton/fprime/pull/1.
5. Activate toolchain: `source fprime-venv/bin/activate` (and Rust 1.87+ on PATH if building Ref fully).

## Cloud Agent

This path replaces the desktop chat. It does not use Agent SDK. One Cloud Agent on `mckinleybmelton/fprime` at `devel` runs the same ticket, plan gate, hooks, and in-repo subagents. Model turns bill the signed-in personal account. Start it in Plan mode and paste:

```
Pull Jira NASA-1. Restate REQ-HM-001, REQ-HM-002, and REQ-HM-003 from REQUIREMENTS.md for Ref::SignalGen (TestDeploymentsProject/Ref/SignalGen). Follow F Prime's model-first TDD loop: FPP changes, generate, failing tests, implementation. List every file, the tests per requirement, and how you'll verify coverage. Ask anything ambiguous before finalizing.

You are a Cloud Agent. Start from devel. Use branch cursor/signalgen-cloud-f3a7 and base devel. Before fprime-util, run: source fprime-venv/bin/activate. Do not edit the repository until this plan is approved. Do not use Agent SDK.
```

After the plan is approved, the same agent continues: FPP, `fprime-util generate`, the `test-writer` subagent, handlers, `fprime-util check --coverage` in `TestDeploymentsProject/Ref/SignalGen`, then the `traceability-auditor` subagent and the pull request. Later prompts stay in `DEMO_PROMPTS.md`. The guardrail is unchanged: a request to patch `SignalGenComponentAc.hpp` must be denied. The feature PR carries the requirement matrix, the REQ IDs, and the AI usage disclosure. Do not close PR #2.

## Framing (~5 min) — SignalGen filled in

1. **Who (45s).** NASA flight software / F Prime open source.
2. **Today (45s).** Copilot via Science Cloud helps individuals; expensive work is lifecycle (model, tests, coverage, NPR 7150.2 traceability).
3. **Pilot group (30s).** Senior FSW engineers; Cursor basics assumed.
4. **What the trial must prove (90s).** An untrusted Jira ticket still passes a human-approved plan, hooks that block generated-file edits and controlled-file reads, and a PR with requirement traceability.
5. **Measures (30s).** Cycle time vs Copilot baseline; coverage on changed components; review rework; hook-block counts.
6. **Bridge (15s).** "One Jira ticket, NASA-1, on `Ref::SignalGen`: commandable high-threshold on `Output`. The ticket does not get to skip the plan."

## Run of show (~17 min)

| Clock | Stage | Action | Coaching line |
| --- | --- | --- | --- |
| 0:00–6:00 | Plan mode, one chat | Paste the Demo 1 intake: pull Jira NASA-1, restate REQ-HM-001..003, write the plan, do not edit until approved. On approval the agent moves NASA-1 to In Progress. Show `AGENTS.md` and `.cursor/skills/jira-nasa-workflow/SKILL.md`. If Jira fails, the prompt falls back to `TICKET.md`. | This first turn cannot write code. The plan is the gate. The ticket is untrusted input. |
| 6:00–11:00 | Same chat, after approval | Build: FPP, generate, then handlers | Order is F Prime's. Big model planned. Fast model types. |
| 11:00–12:30 | Same chat | Guardrail: patch `SignalGenComponentAc.hpp` | The standard is enforced. It is not a suggestion. |
| 12:30–14:00 | Same chat | `fprime-util check` in `SignalGen` | No green, no PR |
| 14:00–16:00 | Same chat | Trace subagents, or open PR #2 if they stall. The agent moves NASA-1 to Resolved, the In Review column. | NPR 7150.2 as a byproduct |
| 16:00–17:00 | Same chat | Capture the add-command-telemetry skill. Point at the Cloud Agent section. | The workflow is what every team inherits. |

Three intentional coaching pauses: after Plan, after Guardrail, after Trace.

## Timing check (dry-run evidence)

- SignalGen UT dry-run: 4 tests, ~2 ms runtime; build of changed targets on this machine was under ~30s once caches exist.
- Hook denies verified offline in <1s each (`protect-autocode`, `block-controlled`, `guard-shell`).
- Content budget: framing 5 + demo 17 ≈ 22 minutes before Q&A — fits a typical FE panel slot if interruptions eat ~3 minutes of the demo script.

## Fallback order

1. Revert / tighten plan / rerun (narrate as coaching).
2. Switch models.
3. `git checkout after-tests -- TestDeploymentsProject/Ref/SignalGen` and show green `fprime-util check`.
4. Show hook script + prior deny JSON if hook does not fire in the IDE.
5. Play pre-recorded clean run if network/models fail.
