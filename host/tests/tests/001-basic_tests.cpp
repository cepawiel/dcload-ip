#include "Tests.hpp"

#include <memory>

#include <TargetConnection.hpp>

using namespace Tests;
// using namespace dctool;

class BasicTests : public TestGroup {
public:
    BasicTests() : TestGroup("basic_tests") {};
    virtual ~BasicTests() {};

    std::unique_ptr<dctool::TargetConnection> TC;
    
protected:
    virtual bool GroupSetup(RunnerConfig& runner) override {
        return runner.TargetUp();
    };

    virtual bool GroupCleanup(RunnerConfig& runner) override {
        return runner.TargetDown();
    };

    virtual bool TestSetup(RunnerConfig& runner) override {
        TC = std::make_unique<dctool::TargetConnection>();
        return TC->Setup({runner.DisplayName, runner.IP});
    };

    virtual bool TestCleanup(RunnerConfig& runner) override {
        TC = nullptr;
        return true;
    };

    virtual std::vector<TestConfig> GetTests() {
        return {
            {
                "Test Version", 
                [this](const GroupConfig&, RunnerConfig&) {
                    auto ver = TC->GetTargetVersion();
                    if(ver) {
                        printf("%s\n", ver->c_str());
                        return true;
                    }
                    return false;
                }
            },
            // {
            //     "Test ", 
            //     [this](const GroupConfig&, RunnerConfig&) {
            //         return true;
            //     }
            // },
        };
    }
};

BasicTests basicTests;