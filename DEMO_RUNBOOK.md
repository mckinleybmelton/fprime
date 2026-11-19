# SignalGen demo runbook (~17 minutes + framing)

Use with `DEMO_PROMPTS.md` and the Cursor public-sector FE interview prep.

## Two PRs (keep them separate in the story)

| PR | Branch | What it proves |
| --- | --- | --- |
| Setup | `cursor/pilot-setup-f241` | Team inherits governed workflow by cloning |
| Ticket dry-run | `cursor/signalgen-dry-run-f241` | One ticket → tested, traced change (bots + matrix) |

## Before the room

1. Privacy Mode on; mention it in framing.
2. Start the **live** demo from the setup branch (workflow only — no SignalGen feature yet):

   ```bash
   git fetch origin
   git checkout cursor/pilot-setup-f241
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

4. Pre-run PR for Bugbot / Security Review: the **dry-run** PR (ticket PR), not the setup PR.
5. Activate toolchain: `source fprime-venv/bin/activate` (and Rust 1.87+ on PATH if building Ref fully).

## Framing (~5 min) — SignalGen filled in

1. **Who (45s).** NASA flight software / F Prime open source.
2. **Today (45s).** Copilot via Science Cloud helps individuals; expensive work is lifecycle (model, tests, coverage, NPR 7150.2 traceability).
3. **Pilot group (30s).** Senior FSW engineers; Cursor basics assumed.
4. **What the trial must prove (90s).** Lifecycle, control (hooks), evidence/repeatability.
5. **Measures (30s).** Cycle time vs Copilot baseline; coverage on changed components; review rework; hook-block counts.
6. **Bridge (15s).** "One realistic ticket on `Ref::SignalGen`: commandable high-threshold on `Output`."

## Run of show (~17 min)

| Clock | Stage | Action | Coaching line |
| --- | --- | --- | --- |
| 0:00–1:30 | Intake | Open setup PR / `TICKET.md`, `REQUIREMENTS.md`, `.cursor/hooks.json` | Rules = guidance; hooks = enforcement; skills load on demand |
| 1:30–5:00 | Plan | Paste Plan prompt; edit plan; approve | Steer at the plan |
| 5:00–10:00 | Build | Paste Build prompt; FPP → generate → tests → impl | F Prime's own TDD loop; big model plan, fast model type |
| 10:00–11:30 | Guardrail | Paste Guardrail prompt | Generated code never hand-edited |
| 11:30–13:30 | Verify | `fprime-util check` in `SignalGen` | No green, no PR |
| 13:30–15:30 | Trace | Open **dry-run ticket PR**; show matrix + bots | NPR 7150.2 as byproduct |
| 15:30–17:00 | Capture | Paste Capture prompt / show backup skill | Process becomes every team's default |

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
