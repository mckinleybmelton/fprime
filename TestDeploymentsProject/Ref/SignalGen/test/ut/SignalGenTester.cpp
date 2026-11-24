// ======================================================================
// \title  SignalGen.hpp
// \author mstarch
// \brief  cpp file for SignalGen test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "SignalGenTester.hpp"

namespace Ref {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

SignalGenTester ::SignalGenTester() : SignalGenGTestBase("Tester", MAX_HISTORY_SIZE), component("SignalGen") {
    this->initComponents();
    this->connectPorts();
    this->m_reqDpBuff.set(this->m_dpBuff, sizeof(this->m_dpBuff));
}

SignalGenTester ::~SignalGenTester() {
    this->component.deinit();
}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void SignalGenTester ::test_start() {
    ASSERT_TLM_Output_SIZE(0);
    sendCmd_Toggle(0, 0);
    component.doDispatch();
    invoke_to_schedIn(0, 0);
    component.doDispatch();
    ASSERT_TLM_Output_SIZE(1);
    sendCmd_Dp(0, 10, Ref::SignalGen_DpReqType::IMMEDIATE, 1, 1);
    component.doDispatch();
    // verify request for data product buffer
    ASSERT_PRODUCT_GET_SIZE(1);
    // run 2 cycles, should output data product on second
    invoke_to_schedIn(0, 0);
    ASSERT_PRODUCT_SEND_SIZE(1);
}

// REQ-HM-001: command sets the high threshold for Output
void SignalGenTester ::testSetHighThreshold() {
    const F32 threshold = 5.0f;
    this->clearHistory();
    this->sendCmd_SetHighThreshold(0, 0, threshold);
    this->component.doDispatch();
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, SignalGen::OPCODE_SETHIGHTHRESHOLD, 0, Fw::CmdResponse::OK);
}

// REQ-HM-002: telemetry reports the active high threshold
void SignalGenTester ::testHighThresholdTelemetry() {
    const F32 threshold = 12.5f;
    this->clearHistory();
    this->sendCmd_SetHighThreshold(0, 0, threshold);
    this->component.doDispatch();
    ASSERT_TLM_HighThreshold_SIZE(1);
    ASSERT_TLM_HighThreshold(0, threshold);

    // Also published each sched cycle while running
    this->sendCmd_Toggle(0, 1);
    this->component.doDispatch();
    this->clearHistory();
    this->invoke_to_schedIn(0, 0);
    ASSERT_TLM_HighThreshold_SIZE(1);
    ASSERT_TLM_HighThreshold(0, threshold);
}

// REQ-HM-003: warning event fires when Output exceeds the configured threshold
void SignalGenTester ::testHighThresholdExceeded() {
    const F32 threshold = 0.5f;
    const F32 amplitude = 2.0f;

    // Before any Output sample, a below-zero threshold must not fabricate a warning
    // from the boot m_lastOutput sentinel (0.0f).
    this->clearHistory();
    this->sendCmd_SetHighThreshold(0, 0, -1.0f);
    this->component.doDispatch();
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(0);

    this->clearHistory();
    this->sendCmd_SetHighThreshold(0, 1, threshold);
    this->component.doDispatch();
    this->sendCmd_Settings(0, 2, 1, amplitude, 0.0f, Ref::SignalType::SQUARE);
    this->component.doDispatch();
    this->sendCmd_Toggle(0, 3);
    this->component.doDispatch();

    this->clearEvents();
    // First sample of SQUARE with positive amplitude is +amplitude (> threshold)
    this->invoke_to_schedIn(0, 0);
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(1);
    ASSERT_EVENTS_HighThresholdExceeded(0, amplitude, threshold);

    // Further samples while still above must not re-emit (once-per-crossing latch)
    this->invoke_to_schedIn(0, 0);
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(1);

    // Raise threshold above current output to re-arm, then lower again to fire once more
    this->sendCmd_SetHighThreshold(0, 4, amplitude + 1.0f);
    this->component.doDispatch();
    this->clearEvents();
    this->sendCmd_SetHighThreshold(0, 5, threshold);
    this->component.doDispatch();
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(1);
}

//! Handle a text event
void SignalGenTester::textLogIn(FwEventIdType id,                //!< The event ID
                                const Fw::Time& timeTag,         //!< The time
                                const Fw::LogSeverity severity,  //!< The severity
                                const Fw::TextLogString& text    //!< The event string
) {
    TextLogEntry e = {id, timeTag, severity, text};

    printTextLogHistoryEntry(e, stdout);
}

Fw::Success::T SignalGenTester ::productGet_handler(FwDpIdType id, FwSizeType dataSize, Fw::Buffer& buffer) {
    printf("Component requested %" PRI_FwSizeType " bytes.\n", dataSize);
    buffer.set(this->m_dpBuff, dataSize);
    this->pushProductGetEntry(id, dataSize);
    return Fw::Success::SUCCESS;
}

}  // end namespace Ref
