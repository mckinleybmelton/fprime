// ======================================================================
// \title  PingReceiverTestMain.cpp
// \author cursor
// \brief  cpp file for PingReceiver component test main function
// ======================================================================

#include "PingReceiverTester.hpp"

// REQ-PR-001: A command sets the ping ceiling.
TEST(REQ_PR_001, SetPingCeiling) {
    Ref::PingReceiverTester tester;
    tester.testSetPingCeiling();
}

// REQ-PR-002: Telemetry reports the active ceiling.
TEST(REQ_PR_002, PingCeilingTelemetry) {
    Ref::PingReceiverTester tester;
    tester.testPingCeilingTelemetry();
}

// REQ-PR-003: A warning event fires when PR_NumPings exceeds the ceiling.
TEST(REQ_PR_003, PingCeilingExceeded) {
    Ref::PingReceiverTester tester;
    tester.testPingCeilingExceeded();
}

TEST(Nominal, StopPingsStillPresent) {
    Ref::PingReceiverTester tester;
    tester.testStopPingsStillPresent();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
