#include "glue/registration.h"
#include "glue/manager.hpp"
#include <iostream>

using namespace std;

void registerGlue(StepInfo *step) {
    cucumber::addStep(step);
}

void unregisterGlue(StepInfo *step) {
    cucumber::removeStep(step);
}
