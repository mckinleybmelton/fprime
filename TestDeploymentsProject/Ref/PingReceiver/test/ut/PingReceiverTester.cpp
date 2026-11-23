// ======================================================================
// \title  PingReceiverTester.cpp
// \author nasa-3
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

// REQ-PR-001
void PingReceiverTester ::testSetPingCeiling() {
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 1, 42);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiver::OPCODE_PR_SETPINGCEILING, 1, Fw::CmdResponse::OK);

    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(0, 2, 7);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiver::OPCODE_PR_SETPINGCEILING, 2, Fw::CmdResponse::OK);
    ASSERT_TLM_PR_PingCeiling(0, 7);
}

// REQ-PR-002
void PingReceiverTester ::testPingCeilingTelemetry() {
    this->clearHistory();
    ASSERT_TLM_PR_PingCeiling_SIZE(0);

    this->sendCmd_PR_SetPingCeiling(0, 1, 42);
    this->component.doDispatch();

    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, 42);

    this->sendCmd_PR_SetPingCeiling(0, 2, 7);
    this->component.doDispatch();

    ASSERT_TLM_PR_PingCeiling_SIZE(2);
    ASSERT_TLM_PR_PingCeiling(1, 7);

    this->clearHistory();
    this->invoke_to_PingIn(0, 7);
    this->component.doDispatch();

    ASSERT_TLM_PR_NumPings(0, 0);
    ASSERT_TLM_PR_PingCeiling_SIZE(0);
}

// REQ-PR-003
void PingReceiverTester ::testPingCeilingExceeded() {
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 1, 1);
    this->component.doDispatch();

    // ping 1: PR_NumPings = 0, below ceiling
    this->invoke_to_PingIn(0, 1);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);

    // ping 2: PR_NumPings = 1, equal to ceiling (strict >)
    this->invoke_to_PingIn(0, 2);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);

    // ping 3: PR_NumPings = 2, first exceedance
    this->invoke_to_PingIn(0, 3);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 2, 1);

    // ping 4: still above ceiling; latch suppresses a second warning
    this->invoke_to_PingIn(0, 4);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_SIZE(1);
    ASSERT_from_PingOut_SIZE(4);

    // re-arm: new ceiling equals last reported count (3)
    this->sendCmd_PR_SetPingCeiling(0, 3, 3);
    this->component.doDispatch();

    // ping 5: PR_NumPings = 4, exceeds re-armed ceiling
    this->invoke_to_PingIn(0, 5);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(2);
    ASSERT_EVENTS_PR_PingCeilingExceeded(1, 4, 3);

    // ping 6: still above; latch holds
    this->invoke_to_PingIn(0, 6);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(2);
}

void PingReceiverTester ::testStopPingsStillInhibits() {
    this->clearHistory();

    this->sendCmd_PR_StopPings(0, 1);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiver::OPCODE_PR_STOPPINGS, 1, Fw::CmdResponse::OK);

    this->invoke_to_PingIn(0, 9);
    this->component.doDispatch();

    ASSERT_from_PingOut_SIZE(0);
    ASSERT_TLM_PR_NumPings_SIZE(1);
    ASSERT_TLM_PR_NumPings(0, 0);
}

void PingReceiverTester ::testDefaultCeilingQuiet() {
    this->clearHistory();

    // Without PR_SetPingCeiling, Health-style early pings must not warn
    for (U32 i = 0; i < 4; i++) {
        this->invoke_to_PingIn(0, i + 1);
        this->component.doDispatch();
    }

    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);
    ASSERT_EVENTS_SIZE(0);
    ASSERT_from_PingOut_SIZE(4);
    ASSERT_TLM_PR_NumPings(3, 3);
}

}  // namespace Ref
