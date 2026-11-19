#!/usr/bin/env bash
# Post-edit formatter — format only changed line ranges, never the whole file.
# Full-file clang-format -i rewrites legacy F Prime spacing (e.g. `Class ::method`)
# and pollutes ticket diffs / breaks follow-up patches.
# afterFileEdit input: file_path + edits[{old_string,new_string}].
set -euo pipefail

input=$(cat)
path=$(echo "$input" | jq -r '.file_path // .path // empty' 2>/dev/null || true)

if [ -z "${path:-}" ] || [ ! -f "$path" ]; then
  exit 0
fi

case "$path" in
  *.c|*.cc|*.cpp|*.cxx|*.h|*.hh|*.hpp|*.hxx) ;;
  *) exit 0 ;;
esac

fmt=""
if command -v clang-format >/dev/null 2>&1; then
  fmt=clang-format
elif [ -x "/workspace/fprime-venv/bin/clang-format" ]; then
  fmt=/workspace/fprime-venv/bin/clang-format
else
  exit 0
fi

# Build --lines=start:end args from edit hunks present in the file after the edit.
# If we cannot locate any hunk, do nothing (do not fall back to whole-file -i).
mapfile -t line_args < <(
  printf '%s' "$input" | python3 -c '
import json, sys
path = sys.argv[1]
data = json.load(sys.stdin)
edits = data.get("edits") or []
if not edits:
    raise SystemExit(0)
try:
    text = open(path, "r", encoding="utf-8", errors="replace").read()
except OSError:
    raise SystemExit(0)
lines = text.splitlines(keepends=True)
offsets = [0]
for line in lines:
    offsets.append(offsets[-1] + len(line))

def line_of(pos: int) -> int:
    for i in range(len(offsets) - 1):
        if offsets[i] <= pos < offsets[i + 1]:
            return i + 1
    return max(len(lines), 1)

ranges = []
for edit in edits:
    new = edit.get("new_string")
    if not new:
        continue
    start = 0
    while True:
        idx = text.find(new, start)
        if idx < 0:
            break
        lo = line_of(idx)
        hi = line_of(idx + max(len(new) - 1, 0))
        ranges.append((lo, hi))
        start = idx + max(len(new), 1)

merged = []
for lo, hi in sorted(set(ranges)):
    if not merged or lo > merged[-1][1] + 1:
        merged.append([lo, hi])
    else:
        merged[-1][1] = max(merged[-1][1], hi)
for lo, hi in merged:
    print(f"--lines={lo}:{hi}")
' "$path"
)

if [ "${#line_args[@]}" -eq 0 ]; then
  exit 0
fi

"$fmt" -i "${line_args[@]}" "$path" 2>/dev/null || true
exit 0
