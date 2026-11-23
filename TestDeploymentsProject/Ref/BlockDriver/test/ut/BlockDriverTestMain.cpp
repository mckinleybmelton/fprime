// ======================================================================
// \title  BlockDriverTestMain.cpp
// \author root
// \brief  cpp file for BlockDriver component test main function
// ======================================================================

#include "BlockDriverTester.hpp"
#include "Fw/Test/UnitTest.hpp"

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

TEST(Nominal, testSetCycleCeiling) {
    REQUIREMENT("REQ-BD-001");
    Ref::BlockDriverTester tester;
    tester.testSetCycleCeiling();
}

TEST(Nominal, testCycleCeilingTelemetry) {
    REQUIREMENT("REQ-BD-002");
    Ref::BlockDriverTester tester;
    tester.testCycleCeilingTelemetry();
}

TEST(Nominal, testCycleCeilingExceeded) {
    REQUIREMENT("REQ-BD-003");
    Ref::BlockDriverTester tester;
    tester.testCycleCeilingExceeded();
}

TEST(Nominal, testCycleCeilingReArm) {
    REQUIREMENT("REQ-BD-003");
    Ref::BlockDriverTester tester;
    tester.testCycleCeilingReArm();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
