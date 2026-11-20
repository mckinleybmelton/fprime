// ======================================================================
// \title  PingReceiverTester.cpp
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

void PingReceiverTester ::testCommandSetsPingCeiling() {
    // REQ-PR-001
    const U32 ceiling = 4;

    this->clearHistory();
    this->sendCmd_PR_SetPingCeiling(0, 0, ceiling);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_SETPINGCEILING, 0, Fw::CmdResponse::OK);
    ASSERT_TLM_PR_PingCeiling_SIZE(1);
    ASSERT_TLM_PR_PingCeiling(0, ceiling);
}

void PingReceiverTester ::testTelemetryReportsPingCeiling() {
    // REQ-PR-002
    this->clearHistory();

    this->sendCmd_PR_SetPingCeiling(0, 0, 9);
    this->component.doDispatch();
    ASSERT_TLM_PR_PingCeiling(0, 9);

    this->sendCmd_PR_SetPingCeiling(0, 1, 2);
    this->component.doDispatch();
    ASSERT_TLM_PR_PingCeiling_SIZE(2);
    ASSERT_TLM_PR_PingCeiling(1, 2);
}

void PingReceiverTester ::testWarningWhenNumPingsExceedsCeiling() {
    // REQ-PR-003
    this->sendCmd_PR_SetPingCeiling(0, 0, 0);
    this->component.doDispatch();
    this->clearHistory();

    this->invoke_to_PingIn(0, 1);
    this->component.doDispatch();
    ASSERT_TLM_PR_NumPings(0, 0);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(0);

    this->invoke_to_PingIn(0, 2);
    this->component.doDispatch();
    ASSERT_TLM_PR_NumPings(1, 1);
    ASSERT_EVENTS_PR_PingCeilingExceeded_SIZE(1);
    ASSERT_EVENTS_PR_PingCeilingExceeded(0, 1, 0);
}

void PingReceiverTester ::testStopPingsStillInhibits() {
    this->clearHistory();
    this->sendCmd_PR_StopPings(0, 0);
    this->component.doDispatch();

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, PingReceiverComponentBase::OPCODE_PR_STOPPINGS, 0, Fw::CmdResponse::OK);

    this->invoke_to_PingIn(0, 7);
    this->component.doDispatch();
    ASSERT_from_PingOut_SIZE(0);
}

#if FW_ENABLE_TEXT_LOGGING
void PingReceiverTester::textLogIn(FwEventIdType id,
                                   const Fw::Time& timeTag,
                                   const Fw::LogSeverity severity,
                                   const Fw::TextLogString& text) {
    TextLogEntry e = {id, timeTag, severity, text};
    this->printTextLogHistoryEntry(e, stdout);
}
#endif

}  // namespace Ref
