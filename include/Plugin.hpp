#ifndef CUCUMBER_CPP_PLUGIN_H
#define CUCUMBER_CPP_PLUGIN_H

#include <engine.hpp>

#include <memory>
#include <vector>
#include <boost/program_options.hpp>

namespace cucumber {

using namespace std;



class InputSource {
public:
    virtual unique_ptr<const Scenario> read() = 0;
};



class OutputSink {
public:
    virtual void write(const ScenarioResult & scenarioResult) = 0;
};



class Plugin {
public:
    virtual const char *name() const = 0;
    virtual ~Plugin() {};
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



vector<unique_ptr<InputPlugin>> & inputPlugins();
vector<unique_ptr<OutputPlugin>> & outputPlugins();

}

#endif //CUCUMBER_CPP_PLUGIN_H
