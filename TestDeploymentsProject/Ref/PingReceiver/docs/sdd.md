# Ref::PingReceiver Component

## 1. Introduction

The `Ref::PingReceiver` is a demonstration component that accepts pings from the [Health Component](../../../../Svc/Health/docs/sdd.md) and has commands to disable ping responses for testing purposes.

## 2. Requirements

The requirements for `Ref::PingReceiver` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
ISF-PNG-001 | The `Ref::PingReceiver` component shall reply to ping requests | System test
ISF-PNG-002 | The `Ref::PingReceiver` component shall provide commands to disable ping responses | System test
REQ-PR-001 | A command sets the ping ceiling | Unit test
REQ-PR-002 | Telemetry reports the active ping ceiling | Unit test
REQ-PR-003 | A warning event fires when `PR_NumPings` exceeds the ceiling | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::PingReceiver` component has the following component diagram:

![`Ref::PingReceiver` Diagram](img/PingReceiverBDD.jpg "Ref::PingReceiver")

## 4. Dictionaries

## 5. Module Checklists

## 6. Unit Testing

| Test | Description | Requirements |
| --- | --- | --- |
| testSetPingCeiling | Command sets the ping ceiling | REQ-PR-001 |
| testPingCeilingTelemetry | Telemetry reports the active ceiling | REQ-PR-002 |
| testPingsExceededWarning | Warning fires when count exceeds ceiling | REQ-PR-003 |

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
11/22/2026 | Commandable ping ceiling (REQ-PR-001..003)



