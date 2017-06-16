#include <plugins/plugins.hpp>

#include <boost/dll/alias.hpp>

#include <iostream>


namespace cucumber {
namespace plugins {


class SomeInputSource : public InputSource {
private:
    int i = 5;
public:
    unique_ptr<const Scenario> read() {
        cerr << "[DEBUG] Reading from Some" << endl;
        if (i > 0)
            return unique_ptr<const Scenario>(new Scenario(to_string(i--)));
        else
            return unique_ptr<const Scenario>();
    };
};

class SomeOutputSink : public OutputSink {
public:
    void write(const ScenarioResult &scenarioResult) {
        std::cout << ">> " << scenarioResult.outcome << std::endl;
    }
};

/**
 * This plugin is an example of both input and output plugin.
 *
 */
class SomePlugin : public InputPlugin, public OutputPlugin {
public:

    const char *name() const {
        return "some";
    }

    unique_ptr<InputSource> inputFor(const string &expression) const {
        if (expression.empty() || expression == "some") {
            return unique_ptr<InputSource>(new SomeInputSource);
        } else {
            throw std::runtime_error("Nothing for me!");
        }
    };

    unique_ptr<OutputSink> outputFor(const string &expression) const {
        unique_ptr<OutputSink> output(new SomeOutputSink);
        return output;
    };

};

namespace {

static RegisterInputPlugin<SomePlugin> input;
static RegisterOutputPlugin<SomePlugin> output;

}

}
}

