# Ref::SignalGen Component

## 1. Introduction

The `Ref::SignalGen` is a demonstration component that generates different waveforms. It is a source for testing plots in the ground software. It also provides a demonstration of data products.

## 2. Requirements

The requirements for `Ref::SignalGen` are as follows:

Requirement | Description | Verification Method
----------- | ----------- | -------------------
REF-SGN-001 | The `Ref::SignalGen` component shall generate telemetry with tunable waveforms | System test
REF-SGN-002 | The `Ref::SignalGen` component shall store waveform data using data products | System test
REQ-HM-001 | The SignalGen component shall provide a command that sets a high threshold for the generated signal output value | Unit test
REQ-HM-002 | The SignalGen component shall report the active high threshold via telemetry | Unit test
REQ-HM-003 | The SignalGen component shall emit a warning-severity event when the current signal output exceeds the configured high threshold | Unit test

## 3. Design

### 3.1 Context

#### 3.1.1 Component Diagram

The `Ref::SignalGen` component has the following component diagram:

![`Ref::SignalGen` Diagram](img/SignalGenBDD.jpg "Ref::SignalGen")

#### 3.1.2 Data Products

The `Ref::SignalGen` component will generate data products using the `Dp` command.
It will demonstrate the two different ways to request data product buffers and will generate
a data product based on storing a commanded number of 

## 4. Dictionaries

### Commands

| Command | Opcode | Description |
| --- | --- | --- |
| `Settings` | 0 | Set frequency, amplitude, phase, and signal type |
| `Toggle` | 1 | Toggle signal generation on/off |
| `Skip` | 2 | Skip the next sample |
| `Dp` | 3 | Request a data product buffer |
| `SetHighThreshold` | 4 | Set the high threshold for `Output` (REQ-HM-001) |

### Telemetry

| Channel | ID | Description |
| --- | --- | --- |
| `Output` | 1 | Current signal Y value |
| `HighThreshold` | 8 | Active high threshold for `Output` (REQ-HM-002) |

### Events

| Event | ID | Severity | Description |
| --- | --- | --- | --- |
| `HighThresholdExceeded` | 9 | warning high | Emitted once per crossing when `Output` exceeds `HighThreshold` (REQ-HM-003) |

Default high threshold at construction is `std::numeric_limits<F32>::max()` so generation does not warn until an operator sets a finite limit. Comparison is strict (`>`). The warning latches until the threshold is raised above the last `Output` value.

## 5. Module Checklists

## 6. Unit Testing

| Test | Requirement |
| --- | --- |
| `TEST(REQ_HM_001, SetHighThreshold)` | REQ-HM-001 |
| `TEST(REQ_HM_002, HighThresholdTelemetry)` | REQ-HM-002 |
| `TEST(REQ_HM_003, HighThresholdExceeded)` | REQ-HM-003 |

## 7. Change Log

Date | Description
---- | -----------
4/20/2017 | Initial Version
4/26/2024 | Added data product demonstration
2026-11-24 | Added commandable high-threshold check (REQ-HM-001..003)



