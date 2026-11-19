# Pilot requirements — SignalGen high-threshold health monitor

Invented for the Cursor public-sector pilot demo. In a real pilot these IDs would come from the team's requirements tracker.

| ID | Statement |
| --- | --- |
| REQ-HM-001 | The SignalGen component shall provide a command (or parameter) that sets a high threshold for the generated signal output value. |
| REQ-HM-002 | The SignalGen component shall report the active high threshold via telemetry. |
| REQ-HM-003 | The SignalGen component shall emit a warning-severity event when the current signal output exceeds the configured high threshold. |

## Verification expectation

Each requirement has at least one unit test that names the REQ ID in a comment and asserts the corresponding command, telemetry, or event behavior using the F Prime generated test harness.
