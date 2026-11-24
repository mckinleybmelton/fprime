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
// Helpers
// ----------------------------------------------------------------------

void PingReceiverTester ::dispatchOne() {
    this->component.doDispatch();
}

void PingReceiverTester ::pingOnce(U32 key) {
    this->invoke_to_PingIn(0, key);
    this->dispatchOne();
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void PingReceiverTester ::testSetPingCeiling() {
    // REQ-PR-001: A command sets the ping ceiling.
    this->clearHistory();

    const U32 ceiling = 5;
    this->sendCmd_PR_SetPingCeiling(0, 10, ceiling);
    this->dispatchOne();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_SETPINGCEILING, 10, Fw::CmdResponse::OK);
}

void PingReceiverTester ::testPingCeilingTelemetry() {
    // REQ-PR-002: Telemetry reports the active ceiling.
    this->clearHistory();

    const U32 ceiling = 42;
    this->sendCmd_PR_SetPingCeiling(0, 20, ceiling);
    this->dispatchOne();

    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, ceiling);
}

void PingReceiverTester ::testPingCeilingExceeded() {
    // REQ-PR-003: A warning event fires when PR_NumPings exceeds the ceiling.
    this->clearHistory();

    // Ceiling 0: first ping publishes PR_NumPings=0 which does not exceed; second publishes 1 which does.
    const U32 ceiling = 0;
    this->sendCmd_PR_SetPingCeiling(0, 30, ceiling);
    this->dispatchOne();
    this->clearEvents();
    this->clearHistory();

    this->pingOnce(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);
    ASSERT_TLM_PR_NumPings_SIZE(1);
    ASSERT_TLM_PR_NumPings(0, 0);

    this->pingOnce(2);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 1, ceiling);

    // Once-per-crossing: further pings while still above do not re-emit.
    this->pingOnce(3);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);

    // Re-arm by raising the ceiling above the current count, then exceed again.
    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(0, 31, 10);
    this->dispatchOne();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);

    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(0, 32, 2);
    this->dispatchOne();
    // Current last-published NumPings is 2 after three pings; lowering to 2 does not exceed (strict >).
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);

    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(0, 33, 1);
    this->dispatchOne();
    // Immediate warning when lowering ceiling below current count.
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 2, 1);
}

void PingReceiverTester ::testStopPingsStillPresent() {
    this->clearHistory();

    this->sendCmd_PR_StopPings(0, 40);
    this->dispatchOne();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_STOPPINGS, 40, Fw::CmdResponse::OK);

    this->clearHistory();
    this->pingOnce(99);
    ASSERT_from_PingOut_SIZE(0);
}

}  // namespace Ref
