#include "Tests.hpp"
#include <stdio.h>

using namespace Tests;

// TODO: Add External Config for 

RunnerConfig dc1 {
    .DisplayName = "Development DC",
    .IP = "172.16.0.99",
    .TargetUp = []() {
        printf("Turning On DC\n");
        return true;
    },
    .TargetDown = []() {
        printf("Turning Off DC\n");
        return true;
    }
};

std::vector<RunnerConfig> runners = { dc1 };

int main() {
    printf("TestRunner\n");
    return TestGroup::TestAllGroups(dc1) ? 0 : -1;
}
