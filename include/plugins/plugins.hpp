#ifndef CUCUMBER_CPP_PLUGIN_H
#define CUCUMBER_CPP_PLUGIN_H

#include <engine.hpp>

#include <boost/filesystem.hpp>

#include <memory>
#include <vector>

using namespace std;

namespace cucumber {
namespace plugins {


class InputSource {
public:
    /**
     * @return the next read scenario, or null pointer if end reached
     */
    virtual unique_ptr<const Scenario> read() = 0;

    virtual ~InputSource() {}
};


class OutputSink {
public:
    /**
     * Acts on the result of a scenario execution.
     *
     * @param scenarioResult
     */
    virtual void write(const ScenarioResult &scenarioResult) = 0;

    virtual ~OutputSink() {}
};


class Plugin {
public:
    virtual const char *name() const = 0;

    virtual ~Plugin() {}
};


class InputPlugin : public Plugin {
public:
    /**
     * Returns an input source from an expression.
     * Throws some exception if it cannot be parsed (not for them)
     * or created (e.g. file not found).
     *
     * @param expression
     * @return
     */
    virtual unique_ptr<InputSource> inputFor(const string &expression) const = 0;
};


class OutputPlugin : public Plugin {
public:
    /**
     * Returns an output source from an expression.
     * Throws some exception if it cannot be parsed or created (e.g. cannot open file).
     *
     * @param expression
     * @return
     */
    virtual unique_ptr<OutputSink> outputFor(const string &expression) const = 0;
};


class BOOST_SYMBOL_EXPORT PluginRegistry {
private:
    vector<InputPlugin *> inputPlugins;
    vector<OutputPlugin *> outputPlugins;

public:
    static PluginRegistry *instance();

    void addInput(InputPlugin *plugin);
//    void removeInput(InputPlugin *plugin);

    InputPlugin & findInputPlugin(const string & name);
    unique_ptr<InputSource> firstInputMatching(const string & expression);

    void addOutput(OutputPlugin *plugin);
//    void removeOutput(OutputPlugin *plugin);

    OutputPlugin & findOutputPlugin(const string & name);
};


template<class T> class RegisterInputPlugin {
private:
    T t;

public:
    RegisterInputPlugin() {
        PluginRegistry::instance()->addInput(&t);
    }
//
//    ~RegisterInputPlugin() {
//        PluginRegistry::instance()->removeInput(&t);
//    }
};

template<class T> class RegisterOutputPlugin {
private:
    T t;

public:
    RegisterOutputPlugin() {
        PluginRegistry::instance()->addOutput(&t);
    }

//    ~RegisterOutputPlugin() {
//        PluginRegistry::instance()->removeOutput(&t);
//    }
};


}
}

#endif //CUCUMBER_CPP_PLUGIN_H
