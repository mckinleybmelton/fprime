# Ref::PingReceiver Component

## 1. Introduction

The `Ref::PingReceiver` is a demonstration component that accepts pings from the [Health Component](../../../../Svc/Health/docs/sdd.md) and has commands to disable ping responses for testing purposes. It also supports a commandable ceiling on the number of received pings, reporting the active ceiling via telemetry and emitting a warning when the count exceeds that ceiling.

## 2. Requirements

The requirements for `Ref::PingReceiver` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
ISF-PNG-001 | The `Ref::PingReceiver` component shall reply to ping requests | System test
ISF-PNG-002 | The `Ref::PingReceiver` component shall provide commands to disable ping responses | System test / Unit test
REQ-PR-001 | The `Ref::PingReceiver` component shall provide a command that sets a ping ceiling | Unit test
REQ-PR-002 | The `Ref::PingReceiver` component shall report the active ping ceiling via telemetry | Unit test
REQ-PR-003 | The `Ref::PingReceiver` component shall emit a warning-severity event when `PR_NumPings` exceeds the configured ceiling | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::PingReceiver` component has the following component diagram:

![`Ref::PingReceiver` Diagram](img/PingReceiverBDD.jpg "Ref::PingReceiver")

### 3.2 Commands

| Command | Opcode | Description |
| --- | --- | --- |
| `PR_StopPings` | 0 | Disable ping responses (preserved; orthogonal to the ceiling) |
| `PR_SetPingCeiling` | 1 | Set the maximum allowed `PR_NumPings` before a warning |

Default ceiling at construction is `U32` max so boot does not emit a startup warning. Changing the ceiling updates `PR_PingCeiling` telemetry. If the new ceiling is below the current `PR_NumPings` value, a single `PR_PingCeilingExceeded` warning is emitted immediately. The warning latch re-arms when the ceiling is raised such that the current count no longer exceeds it.

### 3.3 Telemetry

| Channel | ID | Description |
| --- | --- | --- |
| `PR_NumPings` | 0 | Number of pings received (existing publish semantics) |
| `PR_PingCeiling` | 1 | Active ping ceiling |

### 3.4 Events

| Event | ID | Severity | Description |
| --- | --- | --- | --- |
| `PR_PingsDisabled` | 0 | activity high | Ping responses disabled |
| `PR_PingReceived` | 1 | diagnostic | Ping received |
| `PR_PingCeilingExceeded` | 2 | warning high | `PR_NumPings` exceeded the configured ceiling (once per crossing) |

## 4. Dictionaries

## 5. Module Checklists

## 6. Unit Testing

Unit tests under `test/ut/` cover REQ-PR-001, REQ-PR-002, and REQ-PR-003, and assert that `PR_StopPings` remains available.

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
11/23/2026 | NASA-3: commandable ping ceiling (REQ-PR-001..003)
