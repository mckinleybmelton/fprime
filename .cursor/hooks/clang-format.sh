#!/usr/bin/env bash
# Post-edit formatter — format only a uniquely located edit range.
# Never whole-file clang-format -i (rewrites F Prime `Class ::method` spacing).
# If new_string is missing, empty, tiny, or appears more than once, no-op.
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

# Only format when each new_string uniquely identifies the edited site.
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

MIN_UNIQUE = 8  # skip tiny/common snippets that match everywhere
ranges = []
for edit in edits:
    new = edit.get("new_string") or ""
    old = edit.get("old_string") or ""
    # Prefer a unique anchor: new_string if distinctive, else old->new context.
    anchor = new
    if len(anchor.strip()) < MIN_UNIQUE:
        # Try a bit of surrounding uniqueness from old_string prefix/suffix in file
        # After edit, old is gone; skip rather than guessing.
        continue
    hits = []
    start = 0
    while True:
        idx = text.find(anchor, start)
        if idx < 0:
            break
        hits.append(idx)
        start = idx + max(len(anchor), 1)
        if len(hits) > 1:
            break
    # Ambiguous or missing: do not format (avoids rewriting unrelated copies).
    if len(hits) != 1:
        continue
    idx = hits[0]
    lo = line_of(idx)
    hi = line_of(idx + max(len(anchor) - 1, 0))
    ranges.append((lo, hi))

if not ranges:
    raise SystemExit(0)

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
