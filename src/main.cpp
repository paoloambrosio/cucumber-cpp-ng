#include "extensions.hpp"
#include "plugins/plugins.hpp"

#include <boost/program_options.hpp>
#include <boost/dll.hpp>

#include <iostream>

using namespace std;
using namespace cucumber;
using namespace cucumber::plugins;

namespace po = boost::program_options;
namespace dll = boost::dll;


struct IOSpec {
    IOSpec(const string & spec) {
        auto index = spec.find(':');
        if (index != string::npos) {
            plugin = spec.substr(0, index);
            expression = spec.substr(index + 1, spec.length());
        } else {
            plugin = spec;
            expression = "";
        }
    }

    string plugin;
    string expression;
};


int main(int ac, const char *av[])
{
    try {
        vector<string> in, out, inputSpec;
        vector<boost::filesystem::path> libraryLocations;

        po::options_description visible("");
        visible.add_options()
            ("help,h", "Help message")
            ("load,l", po::value<vector<boost::filesystem::path>>(&libraryLocations)->composing(), "Libraries to load")
            ("in,i", po::value<vector<string>>(&in)->composing(), "Input format")
            ("out,o", po::value<vector<string>>(&out)->composing(), "Output format")
        ;

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


        /*
         * Load extensions
         */

        loadExtension(dll::program_location());
        for (auto & location : libraryLocations) {
            clog << "Loading library from " << location << endl;
            loadExtension(location);
        }

        /*
         * Create input sources
         */

        vector<unique_ptr<InputSource>> inputSources;

        // from explicit parameter
        for (auto & i : in) {
            IOSpec spec(i);
            inputSources.push_back(PluginRegistry::instance()->findInputPlugin(spec.plugin).inputFor(spec.expression));
        }

        // from implicit positional parameter
        for (auto & inputExpr : inputSpec) {
            inputSources.push_back(PluginRegistry::instance()->firstInputMatching(inputExpr));
        }

        /*
         * Create output sinks
         */

        vector<unique_ptr<OutputSink>> outputSinks;
        for (auto & o : out) {
            IOSpec spec(o);
            outputSinks.push_back(PluginRegistry::instance()->findOutputPlugin(spec.plugin).outputFor(spec.expression));
        }

        // TODO if no output is defined use the default? (wire input does not require an output)

        /*
         * Main loop
         */

        for (auto & inputSource : inputSources) {
            while (auto scenario = inputSource->read()) {
                auto result = process(*scenario);
                for (auto & outputSink : outputSinks) {
                    outputSink->write(*result);
                }
            }
        }
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
