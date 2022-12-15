#include "Tests.hpp"

using namespace Tests;

std::vector<TestGroup*> TestGroup::GROUPS = {};

bool TestGroup::TestAllGroups(RunnerConfig& runner) {
    printf("Total Groups to Test: %ld\n", GROUPS.size());
    bool ret = true;
    for( auto group : GROUPS) {
        ret &= group->RunTests(runner);
    }
    
    return ret;
}

bool TestGroup::RunTests(RunnerConfig& runner) {
    auto testName = m_GroupCfg.GroupName.c_str();
    bool failure_detected = false;
    if(!GroupSetup(runner)) {
        printf("Failed to do Group Setup for %s\n", testName);
        return false;
    }

    for(TestConfig& cfg : GetTests()) {
        if(!TestSetup(runner)) {
            printf("Failed to do Group Cleanup for %s\n", testName);
            return false;
        }

        if(!cfg.RunTest(m_GroupCfg, runner)) {
             printf("TEST FAILED: %s\n", testName);
             failure_detected = true;
        }

        TestCleanup(runner);
        if(!TestCleanup(runner)) {
            printf("Failed to do Group Cleanup for %s\n", testName);
        return false;
        }
    }

    if(!GroupCleanup(runner)) {
        printf("Failed to do Group Cleanup for %s\n", testName);
        return false;
    }

    return !failure_detected;
}