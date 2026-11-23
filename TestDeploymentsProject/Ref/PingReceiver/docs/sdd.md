# Ref::PingReceiver Component

## 1. Introduction

The `Ref::PingReceiver` is a demonstration component that accepts pings from the [Health Component](../../../../Svc/Health/docs/sdd.md) and has commands to disable ping responses for testing purposes. It also supports a commandable ceiling on `PR_NumPings` that raises a warning when the reported count exceeds that ceiling.

## 2. Requirements

The requirements for `Ref::PingReceiver` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
ISF-PNG-001 | The `Ref::PingReceiver` component shall reply to ping requests | System test
ISF-PNG-002 | The `Ref::PingReceiver` component shall provide commands to disable ping responses | System test
REQ-PR-001 | A command sets the ping ceiling | Unit test
REQ-PR-002 | Telemetry reports the active ceiling | Unit test
REQ-PR-003 | A warning event fires when `PR_NumPings` exceeds the ceiling | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::PingReceiver` component has the following component diagram:

![`Ref::PingReceiver` Diagram](img/PingReceiverBDD.jpg "Ref::PingReceiver")

### 3.2 Commands

- `PR_StopPings` (opcode 0): inhibits further `PingOut` replies. Does not reset the ping count.
- `PR_SetPingCeiling` (opcode 1): stores the active `PR_NumPings` ceiling, writes `PR_PingCeiling` telemetry, and re-arms the exceedance latch when the last reported count is less than or equal to the new ceiling.

### 3.3 Telemetry

- `PR_NumPings` (id 0): count of pings received (pre-increment value written on each `PingIn`).
- `PR_PingCeiling` (id 1): active ceiling, written only from `PR_SetPingCeiling`.

### 3.4 Events

- `PR_PingsDisabled` / `PR_PingReceived`: existing diagnostic/activity events (unchanged).
- `PR_PingCeilingExceeded` (id 2, severity warning high): emitted once when the reported `PR_NumPings` first strictly exceeds the active ceiling; latched until a re-arming `PR_SetPingCeiling`.

## 4. Dictionaries

## 5. Module Checklists

## 6. Unit Testing

Unit tests under `test/ut/` cover REQ-PR-001..003 and a `PR_StopPings` regression. Run from this module directory:

```bash
fprime-util check -j"$(nproc)"
```

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
11/23/2026 | NASA-3: commandable PR_NumPings ceiling (REQ-PR-001..003)
