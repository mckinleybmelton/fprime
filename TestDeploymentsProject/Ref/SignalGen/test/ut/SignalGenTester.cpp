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
// Helpers
// ----------------------------------------------------------------------

void SignalGenTester ::dispatchCommands() {
    this->component.doDispatch();
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

void SignalGenTester ::testSetHighThreshold() {
    // REQ-HM-001: A command sets the high threshold for Output
    this->clearHistory();
    const F32 threshold = 42.5f;
    this->sendCmd_SetHighThreshold(0, 0, threshold);
    this->dispatchCommands();
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, SignalGen::OPCODE_SETHIGHTHRESHOLD, 0, Fw::CmdResponse::OK);
}

void SignalGenTester ::testHighThresholdTelemetry() {
    // REQ-HM-002: Telemetry reports the active high threshold
    this->clearHistory();
    const F32 threshold = 12.25f;
    this->sendCmd_SetHighThreshold(0, 0, threshold);
    this->dispatchCommands();
    ASSERT_TLM_HighThreshold_SIZE(1);
    ASSERT_TLM_HighThreshold(0, threshold);
}

void SignalGenTester ::testHighThresholdExceeded() {
    // REQ-HM-003: Warning event fires when Output exceeds the configured threshold
    this->clearHistory();

    // Configure a square wave with amplitude above the threshold
    const F32 amplitude = 10.0f;
    const F32 threshold = 5.0f;
    this->sendCmd_Settings(0, 0, 1, amplitude, 0.0f, Ref::SignalType::SQUARE);
    this->dispatchCommands();
    this->sendCmd_SetHighThreshold(0, 0, threshold);
    this->dispatchCommands();
    this->sendCmd_Toggle(0, 0);
    this->dispatchCommands();

    this->clearHistory();
    this->invoke_to_schedIn(0, 0);

    // First square sample at ticks==0 is +amplitude, which exceeds threshold
    ASSERT_TLM_Output_SIZE(1);
    ASSERT_TLM_Output(0, amplitude);
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(1);
    ASSERT_EVENTS_HighThresholdExceeded(0, amplitude, threshold);

    // Further samples while still above must not re-fire (once-per-crossing)
    this->clearHistory();
    this->invoke_to_schedIn(0, 0);
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(0);
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
