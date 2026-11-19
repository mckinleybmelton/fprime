#!/usr/bin/env bash
# Deny edits to F Prime autocoded / build-cache files.
# Covers Write/StrReplace/ApplyPatch and similar: paths may be in tool_input
# fields or only inside an ApplyPatch body (`*** Update File: ...`).
set -euo pipefail

input=$(cat)

# Collect explicit path-like fields when present.
paths=$(echo "$input" | jq -r '
  [
    .tool_input.path?,
    .tool_input.file_path?,
    .tool_input.target_notebook?,
    .tool_input.target_file?,
    .file_path?,
    .path?,
    (.tool_input.paths[]? // empty)
  ] | map(select(. != null and . != "")) | .[]
' 2>/dev/null || true)

# Paths embedded in ApplyPatch / unified-diff style headers inside the payload.
embedded=$(
  printf '%s\n' "$input" \
    | grep -Eo '\*\*\* (Update|Add|Delete) File:[[:space:]]*[^\\"[:space:]]+|(\+\+\+|---) [ab]/[^\\"[:space:]]+' \
    2>/dev/null \
    | sed -E 's/^\*\*\* (Update|Add|Delete) File:[[:space:]]+//; s/^(\+\+\+|---) [ab]\///' \
  || true
)

# Last-resort: any autocode-looking path token anywhere in the tool payload.
fallback=$(
  printf '%s\n' "$input" \
    | grep -Eo '[^[:space:]\\"'\''<>]+(build-fprime-[^[:space:]\\"'\''<>]*|[^[:space:]\\"'\''<>]*Ac\.(hpp|cpp))' \
    2>/dev/null \
  || true
)

check_one() {
  local p="$1"
  p="${p%\"}"
  p="${p#\"}"
  if echo "$p" | grep -Eq 'build-fprime-|Ac\.(hpp|cpp)$|/Ac\.|/build-artifacts/'; then
    return 0
  fi
  return 1
}

blocked=""
while IFS= read -r p; do
  [ -z "$p" ] && continue
  if check_one "$p"; then
    blocked="$p"
    break
  fi
done <<< "$(printf '%s\n%s\n%s\n' "${paths:-}" "${embedded:-}" "${fallback:-}")"

if [ -n "$blocked" ]; then
  jq -n \
    --arg path "$blocked" \
    '{
      permission: "deny",
      user_message: ("Blocked: autocoded/build-cache file: " + $path),
      agent_message: "Generated code is never hand-edited. Change the .fpp (or .fppi) model and run fprime-util generate. Do not patch *Ac.hpp / *Ac.cpp or anything under build-fprime-*."
    }'
  exit 0
fi

echo '{"permission":"allow"}'
