// ======================================================================
// \title  BlockDriverTester.cpp
// \author root
// \brief  cpp file for BlockDriver component test harness implementation class
// ======================================================================

#include "BlockDriverTester.hpp"

namespace Ref {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

BlockDriverTester ::BlockDriverTester()
    : BlockDriverGTestBase("BlockDriverTester", BlockDriverTester::MAX_HISTORY_SIZE), component("BlockDriver") {
    this->initComponents();
    this->connectPorts();
}

BlockDriverTester ::~BlockDriverTester() {
    this->component.deinit();
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void BlockDriverTester ::testDataLoopBack() {
    const U8 data[] = {1, 2, 3, 4, 5, 6, 7};
    Drv::DataBuffer dataBuffer(data, 7);

    this->clearHistory();

    // input data
    this->invoke_to_BufferIn(0, dataBuffer);

    this->component.doDispatch();

    // verify data output
    ASSERT_from_BufferOut_SIZE(1);
    ASSERT_from_BufferOut(0, dataBuffer);
}

void BlockDriverTester ::testPing() {
    const U32 key = 42;

    this->clearHistory();

    // call Ping port with the key
    this->invoke_to_PingIn(0, key);

    this->component.doDispatch();

    // verify Ping output
    ASSERT_from_PingOut_SIZE(1);
    ASSERT_from_PingOut(0, key);
}

void BlockDriverTester ::testCycleIncrement() {
    this->clearHistory();

    // call ISR
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();

    // there shall be one report with 0 cycle
    ASSERT_TLM_SIZE(1);
    ASSERT_TLM_BD_Cycles_SIZE(1);
    ASSERT_TLM_BD_Cycles(0, 0);

    // call ISR once again
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();

    // there shall be one more report with 1 cycle
    ASSERT_TLM_SIZE(2);
    ASSERT_TLM_BD_Cycles_SIZE(2);
    ASSERT_TLM_BD_Cycles(1, 1);
}

// REQ-BD-001
void BlockDriverTester ::testSetCycleCeiling() {
    this->clearHistory();

    this->sendCmd_BD_SET_CYCLE_CEILING(0, 1, 42);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, BlockDriver::OPCODE_BD_SET_CYCLE_CEILING, 1, Fw::CmdResponse::OK);

    this->clearHistory();
    this->sendCmd_BD_SET_CYCLE_CEILING(0, 2, 7);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, BlockDriver::OPCODE_BD_SET_CYCLE_CEILING, 2, Fw::CmdResponse::OK);
    ASSERT_TLM_BD_CycleCeiling(0, 7);
}

// REQ-BD-002
void BlockDriverTester ::testCycleCeilingTelemetry() {
    this->clearHistory();
    ASSERT_TLM_BD_CycleCeiling_SIZE(0);

    this->sendCmd_BD_SET_CYCLE_CEILING(0, 1, 42);
    this->component.doDispatch();

    ASSERT_TLM_BD_CycleCeiling_SIZE(1);
    ASSERT_TLM_BD_CycleCeiling(0, 42);

    this->sendCmd_BD_SET_CYCLE_CEILING(0, 2, 7);
    this->component.doDispatch();

    ASSERT_TLM_BD_CycleCeiling_SIZE(2);
    ASSERT_TLM_BD_CycleCeiling(1, 7);

    this->clearHistory();
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();

    ASSERT_TLM_BD_Cycles(0, 0);
    ASSERT_TLM_BD_CycleCeiling_SIZE(0);
}

// REQ-BD-003
void BlockDriverTester ::testCycleCeilingExceeded() {
    this->clearHistory();

    this->sendCmd_BD_SET_CYCLE_CEILING(0, 1, 1);
    this->component.doDispatch();

    // cycle 1: BD_Cycles = 0, below ceiling
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(0);

    // cycle 2: BD_Cycles = 1, equal to ceiling (strict >)
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(0);

    // cycle 3: BD_Cycles = 2, first exceedance
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_BD_CycleCeilingExceeded(0, 2, 1);

    // cycle 4: still above ceiling; latch suppresses a second warning
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
}

// REQ-BD-003 (latch keep / re-arm)
void BlockDriverTester ::testCycleCeilingReArm() {
    this->clearHistory();

    this->sendCmd_BD_SET_CYCLE_CEILING(0, 1, 0);
    this->component.doDispatch();

    // First cycle reports 0 (does not exceed 0). Second reports 1 and fires once.
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_BD_CycleCeilingExceeded(0, 1, 0);

    // Last reported BD_Cycles is 1. A still-too-low ceiling must keep the latch set.
    this->clearHistory();
    this->sendCmd_BD_SET_CYCLE_CEILING(0, 2, 0);
    this->component.doDispatch();
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(0);

    // Last reported value is 2 (m_cycles == 3). Ceiling 2 re-arms; next sample is 3 and fires.
    this->sendCmd_BD_SET_CYCLE_CEILING(0, 3, 2);
    this->component.doDispatch();
    this->invoke_to_Sched(0, 0);
    this->component.doDispatch();
    ASSERT_EVENTS_BD_CycleCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_BD_CycleCeilingExceeded(0, 3, 2);
}

}  // namespace Ref
