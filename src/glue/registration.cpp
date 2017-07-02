#include "glue/registration.h"
#include <iostream>

using namespace std;

void registerGlue(const StepInfo *step) {
    clog << "**************** Registering " << step->position << endl;
}

void unregisterGlue(const StepInfo *step) {
    clog << "**************** Unegistering " << step->position << endl;
}
