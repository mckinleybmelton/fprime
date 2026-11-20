// ======================================================================
// \title  PingReceiverTester.hpp
// \brief  hpp file for PingReceiver component test harness implementation class
// ======================================================================

#ifndef Ref_PingReceiverTester_HPP
#define Ref_PingReceiverTester_HPP

#include "Ref/PingReceiver/PingReceiver.hpp"
#include "Ref/PingReceiver/PingReceiverGTestBase.hpp"

namespace Ref {

class PingReceiverTester final : public PingReceiverGTestBase {
  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    // Maximum size of histories storing events, telemetry, and port outputs
    static const FwSizeType MAX_HISTORY_SIZE = 10;

    // Instance ID supplied to the component instance under test
    static const FwEnumStoreType TEST_INSTANCE_ID = 0;

    // Queue depth supplied to the component instance under test
    static const FwSizeType TEST_INSTANCE_QUEUE_DEPTH = 10;

  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Construct object PingReceiverTester
    PingReceiverTester();

    //! Destroy object PingReceiverTester
    ~PingReceiverTester();

  public:
    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    //! REQ-PR-001: a command sets the ping ceiling
    void testCommandSetsPingCeiling();

    //! REQ-PR-002: telemetry reports the active ceiling
    void testTelemetryReportsPingCeiling();

    //! REQ-PR-003: a warning event fires when PR_NumPings exceeds the ceiling
    void testWarningWhenNumPingsExceedsCeiling();

    //! PR_StopPings still inhibits ping responses
    void testStopPingsStillInhibits();

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

#if FW_ENABLE_TEXT_LOGGING
    void textLogIn(FwEventIdType id,                //!< The event ID
                   const Fw::Time& timeTag,         //!< The time
                   const Fw::LogSeverity severity,  //!< The severity
                   const Fw::TextLogString& text    //!< The event string
                   ) override;
#endif

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    PingReceiver component;
};

}  // namespace Ref

#endif
