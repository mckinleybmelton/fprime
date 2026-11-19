#!/usr/bin/env bash
# Deny destructive or unexpected outbound shell commands.
# beforeShellExecution input includes command (and optionally working_directory).
set -euo pipefail

input=$(cat)
command=$(echo "$input" | jq -r '.command // .tool_input.command // empty' 2>/dev/null || true)

deny_cmd() {
  local reason="$1"
  jq -n \
    --arg reason "$reason" \
    --arg command "$command" \
    '{
      permission: "deny",
      user_message: ("Blocked shell command: " + $reason),
      agent_message: ("Hook blocked this command (" + $reason + "): " + $command + ". Use a safer alternative or ask the human.")
    }'
  exit 0
}

# Destructive filesystem / git
if echo "$command" | grep -Eq '(^|[[:space:];|&]|/)(rm[[:space:]]+-rf|rm[[:space:]]+-fr)([[:space:];|&]|$)'; then
  deny_cmd "destructive rm -rf"
fi
if echo "$command" | grep -Eq 'git[[:space:]]+push[[:space:]]+.*--force|git[[:space:]]+push[[:space:]]+-f([[:space:]]|$)'; then
  deny_cmd "force push"
fi
if echo "$command" | grep -Eq 'git[[:space:]]+reset[[:space:]]+--hard'; then
  deny_cmd "hard reset"
fi

# Unexpected outbound network (including /usr/bin/curl forms).
if echo "$command" | grep -Eq '(^|[[:space:];|&]|/)(curl|wget|nc|ncat|ssh|scp|sftp)([[:space:];|&]|$)'; then
  deny_cmd "outbound network tool"
fi

# Block shell writes into autocoded / build-cache paths (bypass of preToolUse).
# Deny when the redirect/tee DESTINATION is an autocode path — not merely when
# the command mentions build-fprime while redirecting elsewhere (e.g. 2>/dev/null).
# Covers spaced and glued forms (> >> >| 1> &> >& 2>) into *Ac.* /
# build-fprime-* / build-artifacts. Redirects may sit directly on the
# previous token (`echo>file`). Allows 2>&1 and redirects whose target
# is not an autocode path (`ls build-fprime-… 2>/dev/null`).
if echo "$command" | grep -Eq '(^|[[:space:];|&]|/)tee([[:space:]]|$)' \
  && echo "$command" | grep -Eq 'Ac\.(hpp|cpp)|build-fprime-|/build-artifacts/'; then
  deny_cmd "write to autocoded/build-cache path"
fi
# Longer operators first so >| is not consumed as a bare >.
if echo "$command" | grep -Eq \
  '(>\||&>|>&|1>>|1>|2>>|2>|>>|>)[[:space:]]*[^|&;[:space:]]*(Ac\.(hpp|cpp)|build-fprime-|/build-artifacts/)'; then
  deny_cmd "write to autocoded/build-cache path"
fi

echo '{"permission":"allow"}'
