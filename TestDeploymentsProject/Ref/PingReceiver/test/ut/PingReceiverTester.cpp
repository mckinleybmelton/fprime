// ======================================================================
// \title  PingReceiverTester.cpp
// \author cursor
// \brief  cpp file for PingReceiver component test harness implementation class
// ======================================================================

#include "PingReceiverTester.hpp"

namespace Ref {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

PingReceiverTester ::PingReceiverTester()
    : PingReceiverGTestBase("PingReceiverTester", PingReceiverTester::MAX_HISTORY_SIZE), component("PingReceiver") {
    this->initComponents();
    this->connectPorts();
}

PingReceiverTester ::~PingReceiverTester() {
    this->component.deinit();
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void PingReceiverTester ::testSetPingCeiling() {
    // REQ-PR-001: A command sets the ping ceiling.
    const U32 ceiling = 5;
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 0, ceiling);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_SETPINGCEILING, 0, Fw::CmdResponse::OK);

    // Ceiling is observable via telemetry written by the command handler
    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, ceiling);

    // PR_StopPings remains available beside the new command
    this->clearHistory();
    this->sendCmd_PR_StopPings(0, 1);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_STOPPINGS, 1, Fw::CmdResponse::OK);
}

void PingReceiverTester ::testPingCeilingTelemetry() {
    // REQ-PR-002: Telemetry reports the active ceiling.
    const U32 ceiling = 42;
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 0, ceiling);
    this->component.doDispatch();

    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, ceiling);
}

void PingReceiverTester ::testPingsExceededWarning() {
    // REQ-PR-003: A warning event fires when PR_NumPings exceeds the ceiling.
    const U32 ceiling = 2;
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 0, ceiling);
    this->component.doDispatch();
    this->clearHistory();

    // Pings at or below the ceiling: no warning
    for (U32 i = 0; i < ceiling; i++) {
        this->invoke_to_PingIn(0, i);
        this->component.doDispatch();
    }
    ASSERT_EVENTS_PR_PingsExceeded_SIZE(0);

    // Next ping exceeds the ceiling: one warning
    this->invoke_to_PingIn(0, ceiling);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingsExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingsExceeded(0, ceiling + 1, ceiling);

    // Further pings: still a single edge-triggered warning
    this->invoke_to_PingIn(0, ceiling + 1);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingsExceeded_SIZE(1);
}

}  // namespace Ref
