// ======================================================================
// \title  PingReceiverTestMain.cpp
// \author cursor
// \brief  cpp file for PingReceiver component test main function
// ======================================================================

#include "PingReceiverTester.hpp"

TEST(Nominal, testSetPingCeiling) {
    Ref::PingReceiverTester tester;
    tester.testSetPingCeiling();
}

TEST(Nominal, testPingCeilingTelemetry) {
    Ref::PingReceiverTester tester;
    tester.testPingCeilingTelemetry();
}

TEST(Nominal, testPingsExceededWarning) {
    Ref::PingReceiverTester tester;
    tester.testPingsExceededWarning();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
