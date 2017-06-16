#include "plugins/plugins.hpp"

#include <iostream>

cucumber::plugins::PluginRegistry *cucumber::plugins::PluginRegistry::instance() {
    static PluginRegistry instance;
    return & instance;
}

void cucumber::plugins::PluginRegistry::addInput(InputPlugin *plugin) {
    inputPlugins.push_back(plugin);
}

//void cucumber::plugins::PluginRegistry::removeInput(InputPlugin *plugin) {
//
//}

cucumber::plugins::InputPlugin & cucumber::plugins::PluginRegistry::findInputPlugin(const string & name) {
    for (InputPlugin *plugin : inputPlugins) {
        if (plugin->name() == name) {
            return *plugin;
        }
    }
    cerr << "[WARN] Can't find input plugin " << name << endl;
    throw "Input plugin not found";
}

unique_ptr<cucumber::plugins::InputSource> cucumber::plugins::PluginRegistry::firstInputMatching(const string & expression) {
    for (auto & plugin : inputPlugins) {
        try {
            unique_ptr<InputSource> is = plugin->inputFor(expression);
            cerr << "[DEBUG] Input spec matching plugins " << plugin->name() << endl;
            return is;
        } catch (...) {}
    }
    cerr << "[ERROR] No plugins matching input spec " << expression << endl;
    throw std::runtime_error("No plugins found");
}

void cucumber::plugins::PluginRegistry::addOutput(OutputPlugin *plugin) {
    outputPlugins.push_back(plugin);
}

//void cucumber::plugins::PluginRegistry::removeOutput(OutputPlugin *plugin) {
//
//}

cucumber::plugins::OutputPlugin & cucumber::plugins::PluginRegistry::findOutputPlugin(const string & name) {
    for (OutputPlugin *plugin : outputPlugins) {
        if (plugin->name() == name) {
            return *plugin;
        }
    }
    cerr << "[WARN] Can't find output plugin " << name << endl;
    throw "Output plugin not found";
}
