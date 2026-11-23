// ======================================================================
// \title  PingReceiverTestMain.cpp
// \author nasa-3
// \brief  cpp file for PingReceiver component test main function
// ======================================================================

#include "Fw/Test/UnitTest.hpp"
#include "PingReceiverTester.hpp"

TEST(Nominal, testSetPingCeiling) {
    REQUIREMENT("REQ-PR-001");
    Ref::PingReceiverTester tester;
    tester.testSetPingCeiling();
}

TEST(Nominal, testPingCeilingTelemetry) {
    REQUIREMENT("REQ-PR-002");
    Ref::PingReceiverTester tester;
    tester.testPingCeilingTelemetry();
}

TEST(Nominal, testPingCeilingExceeded) {
    REQUIREMENT("REQ-PR-003");
    Ref::PingReceiverTester tester;
    tester.testPingCeilingExceeded();
}

TEST(Nominal, testStopPingsStillInhibits) {
    COMMENT("Regression: PR_StopPings opcode 0 remains and still inhibits PingOut");
    Ref::PingReceiverTester tester;
    tester.testStopPingsStillInhibits();
}

TEST(Nominal, testDefaultCeilingQuiet) {
    COMMENT("Regression: default ceiling U32 max does not WARNING_HI on early pings");
    Ref::PingReceiverTester tester;
    tester.testDefaultCeilingQuiet();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
