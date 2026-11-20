# Ref::PingReceiver Component

## 1. Introduction

The `Ref::PingReceiver` is a demonstration component that accepts pings from the [Health Component](../../../../Svc/Health/docs/sdd.md) and has commands to disable ping responses for testing purposes.

## 2. Requirements

The requirements for `Ref::PingReceiver` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
ISF-PNG-001 | The `Ref::PingReceiver` component shall reply to ping requests | System test
ISF-PNG-002 | The `Ref::PingReceiver` component shall provide commands to disable ping responses | System test
REQ-PR-001 | The `Ref::PingReceiver` component shall provide a command that sets the ping ceiling | Unit test
REQ-PR-002 | The `Ref::PingReceiver` component shall report the active ping ceiling via telemetry | Unit test
REQ-PR-003 | The `Ref::PingReceiver` component shall emit a warning event when `PR_NumPings` exceeds the ceiling | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::PingReceiver` component has the following component diagram:

![`Ref::PingReceiver` Diagram](img/PingReceiverBDD.jpg "Ref::PingReceiver")

### 3.2 Ping ceiling

`PR_StopPings` remains the command that disables ping responses. `PR_SetPingCeiling` sets a `U32` ceiling and emits telemetry channel `PR_PingCeiling`. Until that command is accepted, the ceiling is the maximum `U32` value. Each ping still updates `PR_NumPings` with the pre-increment count. When that value is greater than the active ceiling, the component emits warning-high event `PR_PingCeilingExceeded`.

## 4. Dictionaries

## 5. Module Checklists

## 6. Unit Testing

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
11/20/2026 | Added commandable ping ceiling (`REQ-PR-001`, `REQ-PR-002`, `REQ-PR-003`)



