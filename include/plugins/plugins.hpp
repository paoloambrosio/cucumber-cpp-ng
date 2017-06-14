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


}
}


// TODO registration macros


#endif //CUCUMBER_CPP_PLUGIN_H
