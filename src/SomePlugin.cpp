#include "Plugin.hpp"

#include <iostream>

using namespace cucumber;


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
    void write(const ScenarioResult & scenarioResult) {
        std::cout << ">> " << scenarioResult.outcome << std::endl;
    }
};

class SomePlugin : public NullPlugin {
private:
    bool some = false;
public:

    const char *name() {
        return "some";
    }

    unique_ptr<InputSource> inputFor(const string & expression) const {
        if (expression.empty() || expression == "some") {
            return unique_ptr<InputSource>(new SomeInputSource);
        } else {
            throw "nothing for me!";
        }
    };

    unique_ptr<OutputSink> outputFor(const string & expression) const {
        unique_ptr<OutputSink> output(new SomeOutputSink);
        return output;
    };

    void processOptions(boost::program_options::options_description & desc) {
        desc.add_options()
            ("some,s", boost::program_options::bool_switch(&some), "do something?")
        ;
    }

    static const bool created;
};

const bool SomePlugin::created = registerPlugin<SomePlugin>();
