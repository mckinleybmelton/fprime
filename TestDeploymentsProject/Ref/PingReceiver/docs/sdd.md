# Ref::PingReceiver Component

## 1. Introduction

The `Ref::PingReceiver` is a demonstration component that accepts pings from the [Health Component](../../../../Svc/Health/docs/sdd.md) and has commands to disable ping responses for testing purposes. A separate command sets a ceiling on the `PR_NumPings` count. Crossing that ceiling emits a warning. It does not stop ping replies.

## 2. Requirements

The requirements for `Ref::PingReceiver` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
ISF-PNG-001 | The `Ref::PingReceiver` component shall reply to ping requests | System test
ISF-PNG-002 | The `Ref::PingReceiver` component shall provide commands to disable ping responses | System test
REQ-PR-001 | A command shall set the ping ceiling | Unit test
REQ-PR-002 | Telemetry shall report the active ping ceiling | Unit test
REQ-PR-003 | A warning event shall fire when `PR_NumPings` exceeds the ceiling | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::PingReceiver` component has the following component diagram:

![`Ref::PingReceiver` Diagram](img/PingReceiverBDD.jpg "Ref::PingReceiver")

### 3.2 Ping ceiling

`PR_SetPingCeiling` stores a `U32` ceiling. The command handler writes `PR_PingCeiling` (`update on change`) and emits `PR_PingCeilingSet`. Monitoring stays disabled until that command succeeds, so the initial ceiling of 0 does not warn and `PR_PingCeiling` is not emitted at construction.

`PingIn` reports `PR_NumPings` with the pre-increment count, then increments. The warning compares that reported value. `PR_PingCeilingExceeded` is emitted from `PingIn` only, and only on the crossing: the reported count is greater than the ceiling, monitoring is enabled, and this ceiling has not already warned. A later report that is not above the ceiling clears that latch. A new `PR_SetPingCeiling` also clears it, so the next ping can warn once if the count is already above the new ceiling. A report equal to the ceiling does not warn.

`PR_StopPings` still only suppresses `PingOut`. The count, the ceiling telemetry, and the warning do not depend on that inhibit.

## 4. Dictionaries

Name | Kind | Id | Description
---- | ---- | -- | -----------
PR_StopPings | Command | 0 | Disable ping responses
PR_SetPingCeiling | Command | 1 | Set the ping ceiling (`ceiling: U32`)
PR_PingsDisabled | Event | 0 | Ping responses disabled
PR_PingReceived | Event | 1 | Ping received
PR_PingCeilingSet | Event | 2 | Activity high. Ceiling was set
PR_PingCeilingExceeded | Event | 3 | Warning high. `PR_NumPings` exceeded the ceiling
PR_NumPings | Telemetry | 0 | Number of pings received
PR_PingCeiling | Telemetry | 1 | Active ping ceiling, update on change

## 5. Module Checklists

## 6. Unit Testing

Unit tests live in `test/ut/` and name REQ-PR-001, REQ-PR-002, and REQ-PR-003. A separate regression checks that `PR_StopPings` still suppresses `PingOut` while `PR_NumPings` continues to update.

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
11/23/2026 | REQ-PR-001..003 commandable ping ceiling, telemetry, and warning



