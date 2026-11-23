// ======================================================================
// \title  PingReceiverTester.cpp
// \author ubuntu
// \brief  cpp file for PingReceiver component test harness implementation class
// ======================================================================

#include "PingReceiverTester.hpp"

#include <Fw/Test/UnitTest.hpp>

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

void PingReceiverTester ::testReqPr001CommandSetsCeiling() {
    // REQ-PR-001
    REQUIREMENT("REQ-PR-001");
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 0, 5);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_SETPINGCEILING, 0, Fw::CmdResponse::OK);
    ASSERT_EVENTS_PR_PingCeilingSet_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingSet(0, 5);

    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 1, 9);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(2);
    ASSERT_CMD_RESPONSE(1, PingReceiverComponentBase::OPCODE_PR_SETPINGCEILING, 1, Fw::CmdResponse::OK);
    ASSERT_EVENTS_PR_PingCeilingSet_SIZE(2);
    ASSERT_EVENTS_PR_PingCeilingSet(1, 9);
}

void PingReceiverTester ::testReqPr002TelemetryReportsCeiling() {
    // REQ-PR-002
    REQUIREMENT("REQ-PR-002");
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 0, 5);
    this->component.doDispatch();

    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, 5);

    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 1, 9);
    this->component.doDispatch();

    ASSERT_TLM_PR_PingCeiling_SIZE(2);
    ASSERT_TLM_PR_PingCeiling(1, 9);
}

void PingReceiverTester ::testReqPr003WarningWhenNumPingsExceedCeiling() {
    // REQ-PR-003
    REQUIREMENT("REQ-PR-003");
    const U32 key = 42;

    // No command yet: monitoring is disarmed, so pings do not warn.
    {
        PingReceiverTester uncommanded;
        uncommanded.clearHistory();
        uncommanded.invoke_to_PingIn(0, key);
        uncommanded.component.doDispatch();
        uncommanded.invoke_to_PingIn(0, key);
        uncommanded.component.doDispatch();
        uncommanded.assertEvents_PR_PingCeilingExceeded_size(__FILE__, __LINE__, 0);
        uncommanded.assertTlm_PR_NumPings(__FILE__, __LINE__, 0, 0);
        uncommanded.assertTlm_PR_NumPings(__FILE__, __LINE__, 1, 1);
    }

    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 0, 1);
    this->component.doDispatch();
    this->clearHistory();

    // Reported 0 and 1 do not exceed ceiling 1. PingOut still echoes.
    this->invoke_to_PingIn(0, key);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);
    ASSERT_TLM_PR_NumPings(0, 0);
    ASSERT_from_PingOut_SIZE(1);
    ASSERT_from_PingOut(0, key);

    this->invoke_to_PingIn(0, key);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);
    ASSERT_TLM_PR_NumPings(1, 1);
    ASSERT_from_PingOut_SIZE(2);
    ASSERT_from_PingOut(1, key);

    // Reported 2 exceeds the ceiling once.
    this->invoke_to_PingIn(0, key);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 2, 1);
    ASSERT_TLM_PR_NumPings(2, 2);
    ASSERT_from_PingOut_SIZE(3);
    ASSERT_from_PingOut(2, key);

    // A later ping that stays above the same ceiling does not emit again.
    this->invoke_to_PingIn(0, key);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_TLM_PR_NumPings(3, 3);
    ASSERT_TLM_PR_PingCeiling_SIZE(0);

    // A new ceiling clears the latch and can warn once on the next ping.
    this->sendCmd_PR_SetPingCeiling(PingReceiverTester::TEST_INSTANCE_ID, 1, 0);
    this->component.doDispatch();
    this->clearHistory();
    this->invoke_to_PingIn(0, key);
    this->component.doDispatch();
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 4, 0);
    ASSERT_from_PingOut_SIZE(1);
    ASSERT_from_PingOut(0, key);
}

void PingReceiverTester ::testStopPingsStillInhibits() {
    this->clearHistory();

    this->sendCmd_PR_StopPings(PingReceiverTester::TEST_INSTANCE_ID, 0);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_STOPPINGS, 0, Fw::CmdResponse::OK);

    this->clearHistory();
    this->invoke_to_PingIn(0, 11);
    this->component.doDispatch();
    ASSERT_from_PingOut_SIZE(0);
    ASSERT_TLM_PR_NumPings_SIZE(1);
    ASSERT_TLM_PR_NumPings(0, 0);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);
}

void PingReceiverTester::textLogIn(FwEventIdType id,
                                   const Fw::Time& timeTag,
                                   const Fw::LogSeverity severity,
                                   const Fw::TextLogString& text) {
    (void)id;
    (void)timeTag;
    (void)severity;
    (void)text;
}

}  // namespace Ref
