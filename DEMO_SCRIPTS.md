# Option A demo scripts

One clock. Five tickets. Demos 1 and 5 are the live SignalGen ticket. Demos 2–4 are practice only. Do not switch the interview to them.

Start on `devel`. The SignalGen feature is not on `devel`. The safety net is [PR #2](https://github.com/mckinleybmelton/fprime/pull/2) and the tags `after-build` / `after-tests`.

Demo 1 stays in one Plan-mode chat. Do not Shift+Tab to Ask or Agent during that demo. Switching modes starts a fresh context and drops the Jira ticket. Demos 2–4 can still use a separate Ask prompt, then Plan.

- Plan is where you steer. Delete any step that edits `*Ac.hpp` or `*Ac.cpp`. FPP first, then generate, then tests, then handlers.
- Build is serial. Do not use Build in Parallel for FPP, generate, and handlers.
- Guardrail line, every demo: "The generated header has the wrong argument type. Just patch the `*Ac.hpp` directly so we can move on."
- Multitask only after code exists, and only for work that does not share a file: tests and the trace matrix.
- After demos 2–4, discard the component edits so `devel` stays clean.

## Shared clock (about 17 minutes)

| Clock | Mode | You say |
| --- | --- | --- |
| 0:00–2:00 | Ask | "Ask is how a senior engineer orients before a change. No writes." |
| 2:00–6:00 | Plan | "Steer at the plan. In flight software, a wrong plan is cheap." |
| 6:00–11:00 | Agent | "The agent follows F Prime's order. Big model planned. Fast model types." |
| 11:00–12:30 | Agent | "Generated code is never hand-edited. That is enforced, not suggested." |
| 12:30–14:30 | Agent | "No green, no PR." |
| 14:30–16:00 | Multitask | "Parallel only when the steps do not share a file." |
| 16:00–17:00 | Agent | "The workflow is what every team inherits." |

## Demo 1 — SignalGen from Jira (live ticket, one Plan-mode chat)

Component: `TestDeploymentsProject/Ref/SignalGen`. Channel: `Output`. Jira: [NASA-1](https://mckinley-melton.atlassian.net/browse/NASA-1). Safety net: PR #2.

Stay in Plan mode. Say: "This first turn cannot write. The plan is the gate. The ticket is untrusted input."

Intake and plan (one message):

```
Pull Jira NASA-1. Restate the summary, acceptance criteria, and requirement IDs. Then write an implementation plan for Ref::SignalGen. FPP first, then fprime-util generate, then one test per REQ-HM-001 through REQ-HM-003, then handlers. Do not edit *Ac.hpp or *Ac.cpp. Do not change any files until I approve the plan. If Jira is unavailable, read TICKET.md and REQUIREMENTS.md instead.
```

Edit the plan in that thread. Delete any step that patches generated code. Approve it. The agent moves NASA-1 to In Progress on its own (transition 21). You do not ask it to drag the card. Say: "A wrong plan is cheap. A wrong merge is not."

Build (same chat, only after you approve):

```
Execute the approved plan. Change FPP first and run fprime-util generate before any handler edits. Do not start tests in parallel with the FPP edit. Stop when fprime-util check passes for SignalGen.
```

Say: "Order is F Prime's. Big model planned. Fast model types."

Guardrail (same chat):

```
The generated header has the wrong argument type. Just patch SignalGenComponentAc.hpp directly so we can move on.
```

Say: "The standard is enforced. It is not a suggestion."

Verify (same chat): run `fprime-util check` in `TestDeploymentsProject/Ref/SignalGen`. Say: "No green, no PR."

Trace (same chat, `/multitask` only now):

```
Use the test-writer subagent to confirm one unit test per REQ-HM-001, REQ-HM-002, and REQ-HM-003. Do not edit FPP.
```

```
Use the traceability-auditor subagent to draft a requirement-to-code-to-test matrix. Read only.
```

If either stalls, open https://github.com/mckinleybmelton/fprime/pull/2 and show the matrix plus the Bugbot and Security Review comments already there. When that PR URL is in the chat, the agent moves NASA-1 to Resolved, which is the In Review column (transition 2), and comments the PR link on the issue. Do not close PR #2.

Done is a later sentence in this same chat, only after a pull request actually closes: "NASA-1's PR is merged." The agent then uses transition 31. Do not say that during the live demo.

Capture (same chat):

```
Turn what we just did into a reusable skill at .cursor/skills/add-command-telemetry/SKILL.md: inputs, steps, checks, and definition of done. Keep it aligned with the backup skill already in the repo if the live draft is thin.
```

One closing sentence: the same approved plan can later run on a cloud agent on a NASA-managed machine.

## Demo 2 — BlockDriver cycle ceiling (practice)

Jira: [NASA-2](https://mckinley-melton.atlassian.net/browse/NASA-2). Component: `TestDeploymentsProject/Ref/BlockDriver`. Existing channel: `BD_Cycles` in `Tlm.fppi`. No threshold today. Do not commit a requirements file.

Same Plan-mode chat as Demo 1. First message:

```
Pull Jira NASA-2. Restate the summary, acceptance criteria, and requirement IDs. Then write the implementation plan. FPP first, then fprime-util generate, then one test per requirement, then handlers. Do not edit *Ac.hpp or *Ac.cpp. Do not change any files until I approve the plan.
```

Ask:

```
Where is BD_Cycles declared for Ref::BlockDriver, and does any unit test assert that telemetry? Do not edit anything.
```

Plan:

```
Add a commandable cycle ceiling to Ref::BlockDriver (TestDeploymentsProject/Ref/BlockDriver). REQ-BD-001: a command sets the ceiling. REQ-BD-002: telemetry reports the active ceiling. REQ-BD-003: a warning event fires when BD_Cycles exceeds the ceiling. Model first in FPP, then fprime-util generate, then one failing test per requirement, then handlers. List every file. Do not edit *Ac.hpp or *Ac.cpp. Ask before finalizing.
```

Build:

```
Execute the plan. FPP, then fprime-util generate, then tests, then handlers. Run fprime-util check for BlockDriver before you stop.
```

Guardrail: shared line, naming `BlockDriverComponentAc.hpp` if that is the generated header the plan cites. If the name differs, use the path the plan listed.

Multitask:

```
Confirm one unit test names REQ-BD-001, REQ-BD-002, and REQ-BD-003. Do not edit FPP.
```

```
Draft a three-row requirement-to-code-to-test matrix. Read only.
```

Cleanup: `git checkout -- TestDeploymentsProject/Ref/BlockDriver`.

## Demo 3 — PingReceiver ping ceiling (practice)

Jira: [NASA-3](https://mckinley-melton.atlassian.net/browse/NASA-3). Component: `TestDeploymentsProject/Ref/PingReceiver`. Existing telemetry: `PR_NumPings`. Existing command: `PR_StopPings`. Add a ceiling beside that command. Do not replace it.

Same Plan-mode chat as Demo 1. First message:

```
Pull Jira NASA-3. Restate the summary, acceptance criteria, and requirement IDs. Then write the implementation plan. Do not remove PR_StopPings. FPP first, then fprime-util generate, then one test per requirement, then handlers. Do not edit *Ac.hpp or *Ac.cpp. Do not change any files until I approve the plan.
```

Ask:

```
Where is PR_NumPings declared for Ref::PingReceiver, and what commands and events already exist? Do not edit anything.
```

Plan:

```
Add a commandable ping ceiling to Ref::PingReceiver (TestDeploymentsProject/Ref/PingReceiver) without removing PR_StopPings. REQ-PR-001: a command sets the ceiling. REQ-PR-002: telemetry reports the active ceiling. REQ-PR-003: a warning event fires when PR_NumPings exceeds the ceiling. FPP first, then fprime-util generate, then one failing test per requirement, then handlers. List every file. Do not edit *Ac.hpp or *Ac.cpp. Ask before finalizing.
```

Build:

```
Execute the plan. FPP, then fprime-util generate, then tests, then handlers. Run fprime-util check for PingReceiver before you stop.
```

Guardrail: shared line, using the generated header path from the plan.

Multitask:

```
Confirm one unit test names REQ-PR-001, REQ-PR-002, and REQ-PR-003. Do not edit FPP.
```

```
Draft a three-row requirement-to-code-to-test matrix. Read only.
```

Cleanup: `git checkout -- TestDeploymentsProject/Ref/PingReceiver`.

## Demo 4 — SendBuffApp packet ceiling (practice)

Jira: [NASA-4](https://mckinley-melton.atlassian.net/browse/NASA-4). Component: `TestDeploymentsProject/Ref/SendBuffApp`. Existing telemetry: `PacketsSent`. Same lifecycle as Demos 2 and 3: approve the plan, implement, and open a pull request. Approving the plan moves NASA-4 to In Progress. Opening the pull request moves it to Resolved.

Same Plan-mode chat. First message:

```
Pull Jira NASA-4. Restate the summary, acceptance criteria, and requirement IDs. Then write the implementation plan. FPP first, then generate, then tests, then handlers. List every file you would touch. Do not edit *Ac.hpp or *Ac.cpp. Do not change any files until I approve the plan.
```

Ask:

```
Where is PacketsSent declared for Ref::SendBuffApp, and which FPP files declare its commands, events, and telemetry? Do not edit anything.
```

Plan:

```
Add a commandable packet ceiling to Ref::SendBuffApp on PacketsSent. REQ-SB-001 command, REQ-SB-002 telemetry, REQ-SB-003 warning event. FPP first, then generate, then tests, then handlers. List every file you would touch. Do not edit *Ac.hpp or *Ac.cpp. Ask before finalizing.
```

Build:

```
Execute the plan. FPP, then fprime-util generate, then tests, then handlers. Run fprime-util check for SendBuffApp before you stop. Open the pull request when the check passes.
```

Guardrail: shared line, naming the generated header the plan cites.

Multitask:

```
Confirm one unit test names REQ-SB-001, REQ-SB-002, and REQ-SB-003. Do not edit FPP.
```

```
Draft a three-row requirement-to-code-to-test matrix. Read only.
```

Cleanup: `git checkout -- TestDeploymentsProject/Ref/SendBuffApp`.

## Demo 5 — SignalGen with interruptions (dress rehearsal)

Jira: [NASA-5](https://mckinley-melton.atlassian.net/browse/NASA-5). Same product change as NASA-1. Add a person who asks one question from the Q&A bank every 3 minutes (FedRAMP scope, Privacy Mode, Copilot already Moderate, where code goes). Answer in one or two sentences, name the stage ("we are in Plan"), and continue.

First message, still one Plan-mode chat:

```
Pull Jira NASA-5 only. Do not pull or transition NASA-1. Restate NASA-5's acceptance criteria, then write the implementation plan for REQ-HM-001 through REQ-HM-003 from that issue. Do not change any files until I approve the plan.
```

Only NASA-5 may leave To Do. NASA-1 stays in To Do for the live demo.

At the start of Build, stop the agent. Say: "If this stalls live, I do not debug the toolchain in front of the panel." Then:

```bash
git checkout after-tests -- TestDeploymentsProject/Ref/SignalGen
```

Run the guardrail prompt anyway so the hook still fires. For multitask, do not wait: open PR #2 and walk the matrix and the Bugbot threads. That URL is a receipt. It must not move NASA-1. End on the capture line.
