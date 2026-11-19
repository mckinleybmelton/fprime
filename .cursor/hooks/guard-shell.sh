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
if echo "$command" | grep -Eq '(^|[[:space:];|&])(rm[[:space:]]+-rf|rm[[:space:]]+-fr)([[:space:];|&]|$)'; then
  deny_cmd "destructive rm -rf"
fi
if echo "$command" | grep -Eq 'git[[:space:]]+push[[:space:]]+.*--force|git[[:space:]]+push[[:space:]]+-f([[:space:]]|$)'; then
  deny_cmd "force push"
fi
if echo "$command" | grep -Eq 'git[[:space:]]+reset[[:space:]]+--hard'; then
  deny_cmd "hard reset"
fi

# Unexpected outbound network from the agent shell (allow package managers used in setup only if needed live; demo blocks curl/wget/nc).
# Also match absolute-path forms (/usr/bin/curl) that skip a bare-token boundary.
if echo "$command" | grep -Eq '(^|[[:space:];|&]|/)(curl|wget|nc|ncat|ssh|scp|sftp)([[:space:];|&]|$)'; then
  deny_cmd "outbound network tool"
fi

# Block shell redirects / tee into autocoded or build-cache paths (bypass of preToolUse).
if echo "$command" | grep -Eq '(>|>>|[[:space:]]tee([[:space:]]|$))' \
  && echo "$command" | grep -Eq 'Ac\.(hpp|cpp)|build-fprime-|/build-artifacts/'; then
  deny_cmd "write to autocoded/build-cache path"
fi

echo '{"permission":"allow"}'
