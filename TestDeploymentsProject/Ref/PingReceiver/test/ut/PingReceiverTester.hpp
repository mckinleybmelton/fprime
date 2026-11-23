// ======================================================================
// \title  PingReceiverTester.hpp
// \author nasa-3
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

    //! REQ-PR-001: command sets the ping ceiling
    void testSetPingCeiling();

    //! REQ-PR-002: telemetry reports the active ceiling
    void testPingCeilingTelemetry();

    //! REQ-PR-003: warning when PR_NumPings exceeds the ceiling
    void testPingCeilingExceeded();

    //! Regression: PR_StopPings still inhibits PingOut
    void testStopPingsStillInhibits();

    //! Regression: default ceiling is quiet until commanded
    void testDefaultCeilingQuiet();

  private:
    // ----------------------------------------------------------------------
    // Helper functions
    // ----------------------------------------------------------------------

    //! Connect ports
    void connectPorts();

    //! Initialize components
    void initComponents();

  private:
    // ----------------------------------------------------------------------
    // Member variables
    // ----------------------------------------------------------------------

    //! The component under test
    PingReceiver component;
};

}  // namespace Ref

#endif
