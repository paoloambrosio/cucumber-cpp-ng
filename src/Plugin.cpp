#include "Plugin.hpp"

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



unique_ptr<InputSource> NullPlugin::inputFor(const string & expression) const {
    throw "some exception";
}

unique_ptr<OutputSink> NullPlugin::outputFor(const string & expression) const {
    throw "some exception";
}

void NullPlugin::processOptions(boost::program_options::options_description & desc) {
}
