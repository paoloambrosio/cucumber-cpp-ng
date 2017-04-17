#ifndef CUCUMBER_CPP_PLUGIN_H
#define CUCUMBER_CPP_PLUGIN_H

#include <memory>
#include <vector>
#include <boost/program_options.hpp>

namespace cucumber {

using namespace std;

class Scenario {
public:
    Scenario(const string & name) :
        name(name) {
    }

    const string name;
};

class ScenarioResult {
public:
    ScenarioResult(const string & outcome) :
        outcome(outcome) {
    }

    const string outcome;
};

class InputSource {
public:
    virtual unique_ptr<const Scenario> read() = 0;
};

InputSource & operator>>(InputSource & is, unique_ptr<const Scenario> scenario);

class OutputSink {
public:
    virtual void write(const ScenarioResult & scenarioResult) = 0;
};

OutputSink & operator<<(OutputSink & os, const ScenarioResult & scenarioResult);


class Plugin {
public:

    virtual const char *name() = 0;

    /**
     * Returns an input source from an expression.
     * Throws some exception if it cannot be parsed (not for them)
     * or created (e.g. file not found).
     *
     * @param expression
     * @return
     */
    virtual unique_ptr<InputSource> inputFor(const string & expression) const = 0;

    /**
     * Returns an output source from an expression.
     * Throws some exception if it cannot be parsed or created (e.g. cannot open file).
     *
     * @param expression
     * @return
     */
    virtual unique_ptr<OutputSink> outputFor(const string & expression) const = 0;

    virtual void processOptions(boost::program_options::options_description & desc) = 0;
};

vector<unique_ptr<Plugin>> & plugins();

template<class P> bool registerPlugin() {
    unique_ptr<P> plugin(new P);
    plugins().push_back(move(plugin));
    return true;
}


class NullPlugin : public Plugin {
public:
    unique_ptr<InputSource> inputFor(const string & expression) const;
    unique_ptr<OutputSink> outputFor(const string & expression) const;
    void processOptions(boost::program_options::options_description & desc);
};


}

#endif //CUCUMBER_CPP_PLUGIN_H
