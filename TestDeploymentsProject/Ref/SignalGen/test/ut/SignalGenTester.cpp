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

// REQ-HM-001
void SignalGenTester ::test_set_high_threshold_command() {
    this->clearHistory();
    sendCmd_SetHighThreshold(0, 0, 1.5f);
    component.doDispatch();
    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, SignalGen::OPCODE_SETHIGHTHRESHOLD, 0, Fw::CmdResponse::OK);
}

// REQ-HM-002
void SignalGenTester ::test_high_threshold_telemetry() {
    this->clearHistory();
    const F32 threshold = 2.25f;
    sendCmd_SetHighThreshold(0, 0, threshold);
    component.doDispatch();
    ASSERT_TLM_HighThreshold_SIZE(1);
    ASSERT_TLM_HighThreshold(0, threshold);
}

// REQ-HM-003
void SignalGenTester ::test_high_threshold_exceeded_event() {
    this->clearHistory();
    // Configure a square wave with amplitude 5 so Output is +5 or -5
    sendCmd_Settings(0, 0, 1, 5.0f, 0.0f, Ref::SignalType::SQUARE);
    component.doDispatch();
    sendCmd_SetHighThreshold(0, 0, 1.0f);
    component.doDispatch();
    sendCmd_Toggle(0, 0);
    component.doDispatch();
    this->clearEvents();
    invoke_to_schedIn(0, 0);
    component.doDispatch();
    ASSERT_EVENTS_HighThresholdExceeded_SIZE(1);
    ASSERT_EVENTS_HighThresholdExceeded(0, 5.0f, 1.0f);
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
