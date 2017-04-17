#include "Plugin.hpp"

#include <iostream>

using namespace cucumber;

// TODO eventually we should return consts to the oustide world
vector<unique_ptr<Plugin>> & cucumber::plugins() {
    static vector<unique_ptr<Plugin>> plugins;
    return plugins;
}


InputSource & operator>>(InputSource & is, unique_ptr<const Scenario> scenario) {
    scenario = is.read();
    return is;
}

OutputSink & operator<<(OutputSink & os, const ScenarioResult & scenarioResult) {
    os.write(scenarioResult);
    return os;
}


unique_ptr<InputSource> NullPlugin::inputFor(const string & expression) {
        unique_ptr<InputSource> empty;
        return empty;
    };

unique_ptr<OutputSink> NullPlugin::outputFor(const string & expression) {
    unique_ptr<OutputSink> empty;
    return empty;
};

void NullPlugin::processOptions(boost::program_options::options_description & desc) {
}

// BEGIN - Plugin examples

class SomeInputSource : public InputSource {
private:
    int i = 5;
public:
    unique_ptr<const Scenario> read() {
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

    unique_ptr<InputSource> inputFor(const string & expression) {
        unique_ptr<InputSource> p(new SomeInputSource);
        return p;
    };

    unique_ptr<OutputSink> outputFor(const string & expression) {
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

// END - Plugin examples
