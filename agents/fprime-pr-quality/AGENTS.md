# fprime-pr-quality

<!-- trigger: synchronize the pull request so the hosted reviewer wakes -->

Agent SDK project (`@grok/agent-sdk`). This file is
for coding agents editing the project. The served agent's prompt is
`bot/instructions.md`.

## Loop

```bash
agent-sdk validate --dir .
agent-sdk info --dir . --json
agent-sdk run --dir . --message "..."
agent-sdk eval --dir .
npm run check
```

Run the CLI under Node, never Bun.

## Layout

- `bot/instructions.md` is the always-on prompt. Keep it short.
- `bot/tools/<name>.ts` is one tool per file. The filename is the tool name.
- `bot/lib/` is shared code. Never discovered.
- `evals/` is filesystem evals (`evals/evals.config.ts` required).
  Not `bot/evals/`. That path is ignored.

## Do not

- Grow host TypeScript for formatting, classification, or reply composition.
  That stays in instructions/skills. Host code owns auth, idempotency,
  evidence seeding, and side-effect gates.
- Add npm deps on a first cut. Stick to what `@grok/agent-sdk` already ships.
- Point `serve` at a parent folder during bring-up. It mounts every sibling.

## GitHub

The channel watches whatever repositories are passed at serve or deploy time. It does not name a GitHub owner in source.

```bash
npx --yes "@grok/agent-sdk@0.7.2" serve --dir . --cursor-events --repo <owner>/fprime
npx --yes "@grok/agent-sdk@0.7.2" deploy --dir . --dry-run
npx --yes "@grok/agent-sdk@0.7.2" deploy --dir . --cursor-events-repo <owner>/fprime
```

Do not deploy from an unattended run. The first deploy prints an alias token once.

The unit-test job that this agent reads is `.github/workflows/pr-quality-ut.yml` at the F Prime repository root. Its check name is `F Prime unit tests`.

## Pointers

- Package loop: `node_modules/@grok/agent-sdk/AGENTS.md`
- Skills: `node_modules/@grok/agent-sdk/skills/` (`create-agent`, `deploy`, `evals`, `hillclimb`, `debug`)
- Docs: `npx @grok/agent-sdk docs`
- LLM docs index: `node_modules/@grok/agent-sdk/dist/docs/llms.txt`
