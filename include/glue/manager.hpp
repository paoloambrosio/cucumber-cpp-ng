#ifndef CUCUMBER_CPP_GLUE_MANAGER_HPP
#define CUCUMBER_CPP_GLUE_MANAGER_HPP

#include "glue/registration.h"

namespace cucumber {

void addStep(StepInfo *step);
void removeStep(StepInfo *step);
void temporaryInvokeStep(const std::string & text); // it should be split into match and invoke

}

#endif //CUCUMBER_CPP_GLUE_MANAGER_HPP
