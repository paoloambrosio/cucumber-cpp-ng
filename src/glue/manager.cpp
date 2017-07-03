#include "glue/manager.hpp"

#include <vector>
#include <iostream>

using namespace std;

static vector<StepInfo *> steps;

void cucumber::addStep(StepInfo *step) {
    steps.push_back(step);
    std::clog << "[DEBUG] Step " << step->position << "@" << step << " added (" << steps.size() << " steps registered)" << std::endl;
}

void cucumber::removeStep(StepInfo *step) {
    steps.erase(std::remove(steps.begin(), steps.end(), step), steps.end());
    std::clog << "[DEBUG] Step " << step->position << "@" << step << " removed (" << steps.size() << " steps registered)" << std::endl;
    // FIXME it doesn't seem to go down to 0
}

void cucumber::temporaryInvokeStep(const std::string & text) {
    auto it = std::find_if(steps.begin(), steps.end(),
                 [&](StepInfo *i) -> bool { return text == i->expression ; });
    // TODO find more than one step logic
    ResultReporter r; // TODO not initialised: will crash if called
    if (it != std::end(steps)) {
        StepInfo *si = *it;
        clog << "[DEBUG] Executing step " << si->position << endl;
        si->body(nullptr, &r);
    } else {
        cerr << "[ERROR] Couldn't find a matching step!" << endl;
    }
}
