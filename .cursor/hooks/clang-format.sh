#!/usr/bin/env bash
# Post-edit formatter only — informs/fixes style after the edit; does not enforce policy.
# afterFileEdit input includes file_path.
set -euo pipefail

input=$(cat)
path=$(echo "$input" | jq -r '.file_path // .path // empty' 2>/dev/null || true)

if [ -z "${path:-}" ] || [ ! -f "$path" ]; then
  exit 0
fi

case "$path" in
  *.c|*.cc|*.cpp|*.cxx|*.h|*.hh|*.hpp|*.hxx)
    if command -v clang-format >/dev/null 2>&1; then
      # Prefer repo-root .clang-format; fail soft if clang-format errors.
      clang-format -i "$path" 2>/dev/null || true
    elif [ -x "/workspace/fprime-venv/bin/clang-format" ]; then
      /workspace/fprime-venv/bin/clang-format -i "$path" 2>/dev/null || true
    fi
    ;;
esac

exit 0
