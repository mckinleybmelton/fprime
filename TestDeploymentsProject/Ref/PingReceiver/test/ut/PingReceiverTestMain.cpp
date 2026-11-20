// ======================================================================
// \title  PingReceiverTestMain.cpp
// \brief  cpp file for PingReceiver component test main function
// ======================================================================

#include "PingReceiverTester.hpp"

TEST(Nominal, testCommandSetsPingCeiling) {
    Ref::PingReceiverTester tester;
    tester.testCommandSetsPingCeiling();
}

TEST(Nominal, testTelemetryReportsPingCeiling) {
    Ref::PingReceiverTester tester;
    tester.testTelemetryReportsPingCeiling();
}

TEST(Nominal, testWarningWhenNumPingsExceedsCeiling) {
    Ref::PingReceiverTester tester;
    tester.testWarningWhenNumPingsExceedsCeiling();
}

TEST(Nominal, testStopPingsStillInhibits) {
    Ref::PingReceiverTester tester;
    tester.testStopPingsStillInhibits();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
