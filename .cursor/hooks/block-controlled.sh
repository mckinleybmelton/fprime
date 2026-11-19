#!/usr/bin/env bash
# Deny reads of export-controlled / CUI-marked content so it cannot enter agent context.
# beforeReadFile supplies file_path and often file bytes in `content`.
# Fail closed: parse errors, missing jq, or empty path => deny (never silent allow).
set -euo pipefail

input=$(cat)

deny_read() {
  local reason="$1"
  local path_arg="${2:-}"
  if command -v jq >/dev/null 2>&1; then
    jq -n \
      --arg reason "$reason" \
      --arg path "$path_arg" \
      '{
        permission: "deny",
        user_message: ("Blocked controlled content read: " + $reason + (if $path == "" then "" else (" (" + $path + ")") end))
      }'
  else
    # Minimal JSON if jq itself is unavailable (still exit 0 with deny body).
    printf '{"permission":"deny","user_message":"Blocked controlled content read: %s"}\n' "$reason"
  fi
  exit 0
}

if ! command -v jq >/dev/null 2>&1; then
  deny_read "jq unavailable"
fi

if ! path=$(printf '%s' "$input" | jq -er '.file_path // .path // empty'); then
  deny_read "invalid hook payload"
fi

# Trim; empty path means we cannot attribute the read — deny.
path=$(printf '%s' "$path" | tr -d '\r')
if [ -z "$path" ]; then
  deny_read "empty file_path"
fi

# Optional inline content from beforeReadFile (do not rely on disk alone).
content=$(printf '%s' "$input" | jq -r '.content // empty' 2>/dev/null || true)

MARK_RE='(^|[[:space:]])(ITAR|EAR99|CUI|EXPORT CONTROLLED)([[:space:]]|$)'

# Path-name markers (demo-friendly and common real markings).
if echo "$path" | grep -Eiq 'ITAR|EAR99|\.EAR\.|/CUI/|\.CUI\.|export-control|controlled-dummy'; then
  deny_read "path marking" "$path"
fi

# Scan payload content when present (covers non-disk / renamed / binary-suffixed paths).
if [ -n "$content" ] && printf '%s' "$content" | grep -Eiq "$MARK_RE"; then
  deny_read "payload content marking" "$path"
fi

# On-disk content scan when the path resolves (text-like suffixes only to avoid huge binaries).
if [ -f "$path" ]; then
  case "$path" in
    *.png|*.jpg|*.jpeg|*.gif|*.webp|*.pdf|*.o|*.a|*.so|*.dylib|*.bin)
      # Binary suffix: rely on path markers + payload content above.
      ;;
    *)
      if head -c 8192 "$path" 2>/dev/null | grep -Eiq "$MARK_RE"; then
        deny_read "on-disk content marking" "$path"
      fi
      ;;
  esac
fi

echo '{"permission":"allow"}'
