---
name: jira-nasa-workflow
description: Move the single NASA Jira issue the user asked to pull. Use when they say Pull Jira NASA-N, approve that plan, open a PR for that issue, or say that issue's PR merged. Do not move other NASA keys mentioned in passing.
---

# NASA board workflow

Site: `https://mckinley-melton.atlassian.net` (cloud id `80295f0d-b913-454f-a049-513af65f7dfe`).

The board column labeled **In Review** is the Jira status **Resolved**. There is no NASA status named In Review. Do not use status id 10002. That status belongs to the KAN project.

Call `transitionJiraIssue` only for the active issue: the single key in the user's latest "Pull Jira NASA-N" message. Mentioning another key, or reading it for background, does not make it active. Never transition NASA-1 unless that pull instruction was NASA-1. A dress rehearsal of NASA-5 must leave NASA-1 in To Do.

Do not ask the user to drag the card. If the call is refused, report the error and the status the issue is still in.

| When | transitionId | Status the card shows |
| --- | --- | --- |
| The user approves the plan | `21` | In Progress |
| A pull request is opened, or the user gives a PR URL | `2` | Resolved, which is the In Review column |
| The user says that pull request is merged or closed | `31` | Done |

## Rules

- Repo files stay untouched until the plan is approved. The Jira transition is not a repo edit, so moving to In Progress still happens at approval.
- NASA-4 is not an exception. After the plan is approved, implement it, open its pull request, and move it to Resolved with transition `2`. Do not stop at In Progress, and do not treat refusing the build as a pass.
- After transition id `2`, add a comment on the issue with the pull request URL. Use `addOrEditJiraIssueComment`. Do not put the comment in the transition `update` field. That field can drop the comment.
- Opening or viewing an existing receipt such as GitHub PR #2 does not by itself move a ticket. Move the active issue to Resolved only when the agent opened that pull request for the active issue, or the user says the URL is the pull request for that issue.
- A pull request closed outside this chat is not visible. Move the active issue to Done only when the user says that issue's pull request is merged or closed.
- Do not close GitHub PR #2 during the live demo. It is the Bugbot receipt. Do not move NASA-1 to Done unless the user says NASA-1's pull request is merged.
