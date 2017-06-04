#ifndef CUCUMBER_CPP_PLUGINREGISTRATION_H
#define CUCUMBER_CPP_PLUGINREGISTRATION_H

#endif //CUCUMBER_CPP_PLUGINREGISTRATION_H

#include <Plugin.hpp>

namespace cucumber {

template<class P> bool registerInputPlugin() {
    inputPlugins().push_back(unique_ptr<InputPlugin>(new P));
    return true;
}

template<class P> bool registerOutputPlugin() {
    outputPlugins().push_back(unique_ptr<OutputPlugin>(new P));
    return true;
}

}