#!/usr/bin/env bash
# Deny reads of export-controlled / CUI-marked content so it cannot enter agent context.
# beforeReadFile input includes file_path.
set -euo pipefail

input=$(cat)
path=$(echo "$input" | jq -r '.file_path // .path // empty' 2>/dev/null || true)

deny_read() {
  local reason="$1"
  jq -n \
    --arg reason "$reason" \
    --arg path "${path:-}" \
    '{
      permission: "deny",
      user_message: ("Blocked controlled content read: " + $reason + " (" + $path + ")")
    }'
  exit 0
}

# Path-name markers (demo-friendly and common real markings).
if echo "${path:-}" | grep -Eiq 'ITAR|EAR99|\.EAR\.|/CUI/|\.CUI\.|export-control|controlled-dummy'; then
  deny_read "path marking"
fi

# Content markers: only scan small text-like files that exist.
if [ -n "${path:-}" ] && [ -f "$path" ]; then
  case "$path" in
    *.png|*.jpg|*.jpeg|*.gif|*.webp|*.pdf|*.o|*.a|*.so|*.dylib|*.bin) ;;
    *)
      if head -c 8192 "$path" 2>/dev/null | grep -Eiq '(^|[[:space:]])(ITAR|EAR99|CUI|EXPORT CONTROLLED)([[:space:]]|$)'; then
        deny_read "content marking"
      fi
      ;;
  esac
fi

echo '{"permission":"allow"}'
