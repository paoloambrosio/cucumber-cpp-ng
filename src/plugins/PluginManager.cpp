#include "plugins/PluginManager.hpp"

#include <boost/dll.hpp>

#include <iostream>

using namespace std;
using namespace cucumber::plugins;
namespace dll = boost::dll;
namespace filesystem = boost::filesystem;

class ExtensionLibrary {
private:
    dll::shared_library lib;
    dll::library_info inf;

public:
    ExtensionLibrary(const filesystem::path path) :
        lib(path),
        inf(path) {
        for (auto & symbol : inf.symbols("CukePlIn")) {
            inputPlugins.push_back(
                    lib.get_alias<InputPlugin *()>(symbol)()
            );
        }
        for (auto & symbol : inf.symbols("CukePlOu")) {
            outputPlugins.push_back(
                    lib.get_alias<OutputPlugin *()>(symbol)()
            );
        }
    }

    // TODO
    vector<InputPlugin *> inputPlugins;
    vector<OutputPlugin *> outputPlugins;

};

static vector<shared_ptr<ExtensionLibrary>> loadedLibs; // it must be outside to be unloaded at the end

void cucumber::plugins::loadPlugins(const filesystem::path path) {
    auto lib = make_shared<ExtensionLibrary>(path);
    loadedLibs.push_back(lib);
}

unique_ptr<InputSource> cucumber::plugins::firstInputMatching(const string & expression) {
    for (auto & extension : loadedLibs) {
        for (auto & plugin : extension->inputPlugins) {
            try {
                unique_ptr<InputSource> is = plugin->inputFor(expression);
                cerr << "[DEBUG] Input spec matching plugins " << plugin->name() << endl;
                return is;
            } catch (...) {}
        }
    }
    cerr << "[ERROR] No plugins matching input spec " << expression << endl;
    throw std::runtime_error("No plugins found");
}

InputPlugin & cucumber::plugins::findInputPlugin(const string & name) {
    for (auto & extension : loadedLibs) {
        for (InputPlugin *plugin : extension->inputPlugins) {
            if (plugin->name() == name) {
                return *plugin;
            }
        }
    }
    cerr << "[WARN] Can't extract plugins name from " << name << endl;
    throw "Input plugin not found";
}

OutputPlugin & cucumber::plugins::findOutputPlugin(const string & name) {
    for (auto & extension : loadedLibs) {
        for (OutputPlugin *plugin : extension->outputPlugins) {
            if (plugin->name() == name) {
                return *plugin;
            }
        }
    }
    cerr << "[WARN] Can't extract plugins name from " << name << endl;
    throw "Output plugin not found";
}
