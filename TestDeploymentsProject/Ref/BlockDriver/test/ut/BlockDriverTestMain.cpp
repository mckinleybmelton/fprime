// ======================================================================
// \title  BlockDriverTestMain.cpp
// \author root
// \brief  cpp file for BlockDriver component test main function
// ======================================================================

#include "BlockDriverTester.hpp"

TEST(Nominal, testDataLoopBack) {
    Ref::BlockDriverTester tester;
    tester.testDataLoopBack();
}

TEST(Nominal, testPing) {
    Ref::BlockDriverTester tester;
    tester.testPing();
}

TEST(Nominal, testCycleIncrement) {
    Ref::BlockDriverTester tester;
    tester.testCycleIncrement();
}

// REQ-BD-001
TEST(CycleCeiling, SetCommand) {
    Ref::BlockDriverTester tester;
    tester.test_set_cycle_ceiling_command();
}

// REQ-BD-002
TEST(CycleCeiling, Telemetry) {
    Ref::BlockDriverTester tester;
    tester.test_cycle_ceiling_telemetry();
}

// REQ-BD-003
TEST(CycleCeiling, ExceededEvent) {
    Ref::BlockDriverTester tester;
    tester.test_cycle_ceiling_exceeded_event();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
