#include "PluginRegistration.hpp"

#include "WirePlugin.hpp"

using namespace cucumber;

/*
 * These plugins are loaded by default. We could just use autoregistration
 * and enable them in a configuration file, along with the ordering.
 */
vector<unique_ptr<InputPlugin>> defaultInputPlugins() {
    vector<unique_ptr<InputPlugin>> plugins;
    plugins.push_back(unique_ptr<InputPlugin>(new WireProtocolPlugin));
    return plugins;
}

vector<unique_ptr<InputPlugin>> & cucumber::inputPlugins() {
    static vector<unique_ptr<InputPlugin>> plugins(defaultInputPlugins());
    return plugins;
}



vector<unique_ptr<OutputPlugin>> & cucumber::outputPlugins() {
    static vector<unique_ptr<OutputPlugin>> plugins;
    return plugins;
}
