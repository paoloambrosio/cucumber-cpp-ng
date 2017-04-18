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
        vector<unique_ptr<InputSource>> inputSources;
        vector<unique_ptr<OutputSink>> outputSinks;


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
            ("out,o", po::value<vector<string>>(&out)->composing(), "Output format")
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

        for (auto i = out.begin(); i != out.end(); ++i) {
            auto index = i->find(':');

            string pluginName, outputExpr;
            if (index != string::npos) {
                pluginName = i->substr(0, index);
                outputExpr = i->substr(index + 1, i->length());
            } else {
                pluginName = *i;
                outputExpr = "";
            }

            outputSinks.push_back(findPlugin(pluginName).outputFor(outputExpr));
        }
        // TODO if no output is defined use the default?

        /*
         * TODO ============> Glue
         */

        for (auto is = inputSources.begin(); is != inputSources.end(); ++is) {
            auto & inputSource = *is;

            while (unique_ptr<const Scenario> scenario = inputSource->read()) {
                unique_ptr<const ScenarioResult> result = process(*scenario);

                for (auto os = outputSinks.begin(); os != outputSinks.end(); ++os) {
                    auto & outputSink = *os;
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
