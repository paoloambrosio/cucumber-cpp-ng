#include <iostream>
#include "Plugin.hpp"
#include <boost/program_options.hpp>

using namespace std;
using namespace cucumber;

namespace po = boost::program_options;

namespace {

    Plugin & findPlugin(const string & name) {
        for (auto i = plugins().begin(); i != plugins().end(); ++i) {
            auto & plugin = (*i);
            if (plugin->name() == name) {
                return *plugin;
            }
        }
        cerr << "[WARN] Can't extract plugin name from " << name << endl;
        throw "Plugin not found";
    }

    unique_ptr<InputSource> inputMatching(const string & expression) {
        for (auto p = plugins().begin(); p != plugins().end(); ++p) {
            auto & plugin = (*p);
            try {
                unique_ptr<InputSource> is = plugin->inputFor(expression);
                cerr << "[DEBUG] Input spec matching plugin " << plugin->name() << endl;
                return is;
            } catch (...) {}
        }
        cerr << "[ERROR] No plugin matching input spec " << expression << endl;
        throw "No plugin found";
    }
}

int main(int ac, const char *av[])
{
    try {
        vector<string> glue, in, out, inputSpec;

        cerr << "[DEBUG] Registered plugins:";
        for (auto i = plugins().begin(); i != plugins().end(); ++i) {
            auto & plugin = (*i);
            cerr << " " << plugin->name();
        }
        cerr << endl;


        po::options_description visible("");
        visible.add_options()
            ("help,h", "produce help message")
//            ("glue,g", po::value<vector<string>>(&glue)->composing(), "Read step definitions from those libraries")
            ("in,i", po::value<vector<string>>(&in)->composing(), "input format")
//            ("out,o", po::value<vector<string>>(&out)->composing(), "Output format")
        ;

        for (auto i = plugins().begin(); i != plugins().end(); ++i) {
            auto & plugin = (*i);
            plugin->processOptions(visible);
        }

        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-spec", po::value<vector<string>>(&inputSpec)->composing())
        ;

        po::options_description all;
        all.add(visible).add(hidden);

        po::positional_options_description p;
        p.add("input-spec", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).options(all).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << visible << endl;
            return 1;
        }

        vector<unique_ptr<InputSource>> inputSources;
        for (auto i = in.begin(); i != in.end(); ++i) {
            auto index = i->find(':');

            string pluginName, inputExpr;
            if (index != string::npos) {
                pluginName = i->substr(0, index);
                inputExpr = i->substr(index + 1, i->length());
            } else {
                pluginName = *i;
                inputExpr = "";
            }

            inputSources.push_back(findPlugin(pluginName).inputFor(inputExpr));
        }

        for (auto i = inputSpec.begin(); i != inputSpec.end(); ++i) {
            auto & inputExpr = (*i);
            inputSources.push_back(inputMatching(inputExpr));
        }

        for (auto is = inputSources.begin(); is != inputSources.end(); ++is) {
            auto & inputSource = *is;

            for (unique_ptr<const Scenario> scenario = inputSource->read(); scenario; scenario = inputSource->read()) {
                cout << "Read " << scenario->name << endl;
            }
        }

        /*
         * Extra parameters registered by plugins.
         *  - Can be on separate sections of not, depending how core the plugin is.
         *
         * - Input expressions are registered without plugin? (e.g. file.feature -> :file.feature)
         * - Pass to named plugin inputs and outputs for that one
         *   - Exit on any error
         * - Pass to all plugins if none is specified for an input
         *   - Stop at the first that works
         *   - Exit on error creating input source
         *   - Exit if no plugin is able to parse it (e.g. feature file and no gherkin plugin loaded)
         *
         * Order of plugins:
         *  - Wire (for integer ports)
         *  - Gherkin (anything else interpreted as file)
         *
         * ISSUE: The wire protocol first matches the step(s?) and then executes it! How can we do that?
         *        We could use notifications to output to the wire protocol, but can't really use an input
         *        source. Perhaps we just need to give up on that design and pass the cucumber engine to
         *        input plugins for them to drive?
         */
    }
    catch(std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!" << endl;
        return 1;
    }

    return 0;
}
