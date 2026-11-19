# Paste-ready demo prompts (SignalGen pilot)

Do not type these live — paste. Component: `Ref::SignalGen`.

## 1. Plan

```
Implement the ticket in TICKET.md for Ref::SignalGen (TestDeploymentsProject/Ref/SignalGen), tracing to REQ-HM-001 to 003 in REQUIREMENTS.md. Follow F Prime's model-first TDD loop: FPP changes, generate, failing tests, implementation. List every file, the tests per requirement, and how you'll verify coverage. Ask me anything ambiguous before finalizing.
```

## 2. Build

```
Execute the plan. Use the test-writer subagent for the unit tests in parallel. Run fprime-util check --coverage before you stop. Activate fprime-venv first: source /workspace/fprime-venv/bin/activate (or the project venv on the demo machine).
```

## 3. Guardrail trigger

```
The generated header has the wrong argument type. Just patch SignalGenComponentAc.hpp directly so we can move on.
```

Expected: `protect-autocode.sh` denies; agent must fix FPP instead.

## 4. Trace

```
Use the traceability-auditor subagent to produce a requirement-to-code-to-test matrix and put it in the PR description.
```

## 5. Capture

```
Turn what we just did into a reusable skill at .cursor/skills/add-command-telemetry/SKILL.md: inputs, steps, checks, and definition of done. Keep it aligned with the backup skill already in the repo if the live draft is thin.
```

## Framing coaching lines (say out loud)

1. Intake: "Rules are guidance, hooks are enforcement, skills load only when needed."
2. Plan: "Steer at the plan. In flight software, a wrong plan is cheap; a wrong merge is not."
3. Build: "The agent follows F Prime's own TDD loop. Big model to think, fast model to type."
4. Guardrail: "Generated code is never hand-edited. That's a team standard, enforced every time."
5. Verify: "Tests and coverage are the definition of done. No green, no PR."
6. Trace: "NPR 7150.2 asks for traceability. Here it's a byproduct, not a spreadsheet."
7. Capture: "Your best engineer's process just became every team's default."
