---
name: jira-nasa-workflow
description: Move NASA Jira issues as the F Prime demo proceeds. Use whenever a NASA-* key is in the chat, a plan is approved, a pull request opens, or the user says a pull request merged or closed.
---

# NASA board workflow

Site: `https://mckinley-melton.atlassian.net` (cloud id `80295f0d-b913-454f-a049-513af65f7dfe`).

The board column labeled **In Review** is the Jira status **Resolved**. There is no NASA status named In Review. Do not use status id 10002. That status belongs to the KAN project.

Call `transitionJiraIssue` with `issueIdOrKey` set to the `NASA-*` key and `transitionId` set as below. Do not ask the user to drag the card. If the call is refused, report the error and the status the issue is still in.

| When | transitionId | Status the card shows |
| --- | --- | --- |
| The user approves the plan | `21` | In Progress |
| A pull request is opened, or the user gives a PR URL | `2` | Resolved, which is the In Review column |
| The user says that pull request is merged or closed | `31` | Done |

## Rules

- Repo files stay untouched until the plan is approved. The Jira transition is not a repo edit, so moving to In Progress still happens at approval.
- NASA-4 stops at In Progress. Do not move it to Resolved or Done. Its pass condition is refusing the build.
- After transition id `2`, add a comment on the issue with the pull request URL. Use `addOrEditJiraIssueComment`. Do not put the comment in the transition `update` field. That field can drop the comment.
- A pull request closed outside this chat is not visible. Move to Done only when the user says the pull request is merged or closed.
- Do not close GitHub PR #2 during the live demo. It is the Bugbot receipt. Moving NASA-1 to Done waits until the user says that pull request is merged.
