#ifndef CUCUMBER_CPP_ENGINE_H
#define CUCUMBER_CPP_ENGINE_H

#include <memory>
#include <string>

namespace cucumber {

using namespace std;

class Scenario {
public:
    Scenario(const string &name) :
            name(name) {
    }

    const string name;
};

class ScenarioResult {
public:
    ScenarioResult(const string &outcome) :
            outcome(outcome) {
    }

    const string outcome;
};

// Move to an object at least for testability

// These are just stubs to show where they should be called
void matchStep();
void beginScenario();
void invokeStep();
void endScenario();

unique_ptr<const ScenarioResult> process(const Scenario & scenario);

}

#endif //CUCUMBER_CPP_ENGINE_H
