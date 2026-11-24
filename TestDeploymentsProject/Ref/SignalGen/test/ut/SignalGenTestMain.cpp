// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "SignalGenTester.hpp"

TEST(Nominal, TestStart) {
    Ref::SignalGenTester tester;
    tester.test_start();
}

// REQ-HM-001
TEST(REQ_HM_001, SetHighThreshold) {
    Ref::SignalGenTester tester;
    tester.testSetHighThreshold();
}

// REQ-HM-002
TEST(REQ_HM_002, HighThresholdTelemetry) {
    Ref::SignalGenTester tester;
    tester.testHighThresholdTelemetry();
}

// REQ-HM-003
TEST(REQ_HM_003, HighThresholdExceeded) {
    Ref::SignalGenTester tester;
    tester.testHighThresholdExceeded();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
