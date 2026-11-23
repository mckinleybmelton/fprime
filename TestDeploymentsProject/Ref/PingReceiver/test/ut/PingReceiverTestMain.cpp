// ======================================================================
// \title  PingReceiverTestMain.cpp
// \author ubuntu
// \brief  cpp file for PingReceiver component test main function
// ======================================================================

#include "PingReceiverTester.hpp"

TEST(Nominal, REQ_PR_001) {
    Ref::PingReceiverTester tester;
    tester.testReqPr001CommandSetsCeiling();
}

TEST(Nominal, REQ_PR_002) {
    Ref::PingReceiverTester tester;
    tester.testReqPr002TelemetryReportsCeiling();
}

TEST(Nominal, REQ_PR_003) {
    Ref::PingReceiverTester tester;
    tester.testReqPr003WarningWhenNumPingsExceedCeiling();
}

TEST(Nominal, StopPingsStillInhibits) {
    Ref::PingReceiverTester tester;
    tester.testStopPingsStillInhibits();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
