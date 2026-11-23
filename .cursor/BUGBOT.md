# F Prime Bugbot rules — requirements traceability

Pilot / NPR-style reviews: when a PR is in scope for requirements, every
in-scope REQ ID must map to FPP, implementation, and a unit test that cites
the ID. Prefer evidence from the PR diff over speculation. Never treat
autocoded `*Ac.*` files as the model source of truth.

## Requirements in scope

If the PR touches `REQUIREMENTS.md`, `TICKET.md`, or any changed file (or the
PR body) cites a `REQ-*` ID — including `REQUIREMENT("REQ-...")` macros or
REQ comments — treat those REQ IDs as **in scope** for the rules below.

## REQ → test

If any in-scope REQ ID has no unit-test evidence in the PR that cites that
ID via `REQUIREMENT("REQ-...")` or an explicit REQ comment (e.g.
`// REQ-HM-001`), then:

- Add a blocking Bug titled "Missing test citation for requirement"
- Body: "In-scope requirement `<REQ-ID>` has no unit test in this PR that
  cites the ID with `REQUIREMENT(\"<REQ-ID>\")` or an explicit REQ comment.
  Add or update a test that names the requirement."
- Apply label "traceability"

## REQ → FPP (model first)

If an in-scope REQ implies a new or changed command, telemetry channel,
event, or parameter, and the PR adds or changes handler or threshold logic
in component `.cpp` / `.hpp` files without a corresponding `.fpp` / `.fppi`
declaration in the diff, then:

- Add a blocking Bug titled "Implementation without FPP model update"
- Body: "Requirement `<REQ-ID>` needs a model-first change. Update the
  component `.fpp` / `.fppi` (command, telemetry, event, or param) before
  implementing handlers. Do not hand-edit `*Ac.hpp` / `*Ac.cpp`."
- Apply label "traceability"

## REQ → implementation

If an in-scope REQ has an FPP declaration in the PR (or clearly relies on a
new model symbol) but the PR has no matching handler or logic site in the
component `.cpp` / `.hpp`, then:

- Add a blocking Bug titled "Requirement declared but unimplemented"
- Body: "Requirement `<REQ-ID>` is declared in FPP but this PR has no
  matching implementation in the component `.cpp` / `.hpp`. Add the handler
  or threshold logic that satisfies the requirement."
- Apply label "traceability"

## PR matrix

If any REQ ID is in scope and the PR description lacks a
requirement-to-code-to-test matrix with columns for Requirement, FPP,
Implementation, and Unit tests, then:

- Add a non-blocking Bug titled "Missing requirement-to-code-to-test matrix"
- Body: "This PR is in scope for requirements but the description has no
  matrix mapping each REQ ID to FPP declaration, implementation site, and
  unit test(s). Add a Markdown table (same shape as the
  traceability-auditor agent output)."
- Apply label "traceability"

## Autocode edits

If the PR modifies any `*Ac.hpp`, `*Ac.cpp`, or any path under
`build-fprime-*`, then:

- Add a blocking Bug titled "Hand-edited autocoded file"
- Body: "Autocoded files must not be edited by hand. Change the `.fpp` /
  `.fppi` model and regenerate with `fprime-util generate` instead."
- Apply label "traceability"
