#pragma once

#include <functional>
#include <vector>
#include <string>

namespace Tests {

struct RunnerConfig {
    std::string DisplayName;
    std::string IP;
    std::function<bool()> TargetUp;
    std::function<bool()> TargetDown;
};

struct GroupConfig {
    std::string GroupName;
    int Priority;
};

typedef std::function<bool(const GroupConfig&, RunnerConfig&)> TestCB;

struct TestConfig {
    std::string TestName;
    TestCB RunTest;
};

class TestGroup {
private:
    static std::vector<TestGroup*> GROUPS;

    GroupConfig m_GroupCfg;
    

protected:
    // Run at begining and end of group
    virtual bool GroupSetup(RunnerConfig&) { return true; };
    virtual bool GroupCleanup(RunnerConfig&) { return true; };

    // Run between every test
    virtual bool TestSetup(RunnerConfig&) { return true; };
    virtual bool TestCleanup(RunnerConfig&) { return true; };

public:
    virtual std::vector<TestConfig> GetTests() { return {}; };

    TestGroup(const std::string name, int priority = 0)
        : m_GroupCfg({name, priority}) {
        GROUPS.push_back(this);
    }

    virtual ~TestGroup() {
        std::erase_if(GROUPS, [this](auto a) { return a == this; });
    }

    std::string GetGroupName() { return m_GroupCfg.GroupName; }

    static bool TestAllGroups(RunnerConfig& runner);
    // static bool TestAllGroups(std::vector<RunnerConfig> runners);
    bool RunTests(RunnerConfig& runner);
};

}