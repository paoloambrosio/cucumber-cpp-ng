#include <engine.hpp>

#include <iostream>

using namespace cucumber;
using namespace std;

unique_ptr<const ScenarioResult> cucumber::process(const Scenario & scenario) {
    cerr << "<< " << scenario.name << endl;
    matchStep();
    matchStep();
    beginScenario();
    invokeStep();
    invokeStep();
    endScenario();
    return unique_ptr<const ScenarioResult>(new ScenarioResult("success " + scenario.name));
}

void cucumber::matchStep() {
    cerr << "[DEBUG] Match step" << endl;
}
void cucumber::beginScenario() {
    cerr << "[DEBUG] Begin scenario" << endl;
}
void cucumber::invokeStep() {
    cerr << "[DEBUG] Invoke" << endl;
}
void cucumber::endScenario() {
    cerr << "[DEBUG] End scenario" << endl;
}
