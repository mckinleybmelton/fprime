// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "SignalGenTester.hpp"

TEST(Nominal, TestStart) {
    Ref::SignalGenTester tester;
    tester.test_start();
}

// REQ-HM-001
TEST(HighThreshold, SetCommand) {
    Ref::SignalGenTester tester;
    tester.test_set_high_threshold_command();
}

// REQ-HM-002
TEST(HighThreshold, Telemetry) {
    Ref::SignalGenTester tester;
    tester.test_high_threshold_telemetry();
}

// REQ-HM-003
TEST(HighThreshold, ExceededEvent) {
    Ref::SignalGenTester tester;
    tester.test_high_threshold_exceeded_event();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
