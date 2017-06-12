#include <plugins/plugins.hpp>

#include <boost/dll/alias.hpp>

#include <iostream>
#include <regex>

using namespace std;
using namespace cucumber;
using namespace cucumber::plugins;

namespace {

enum MessageType {
    match_step,
    begin_scenario,
    invoke_step,
    end_scenario
};

class WireProtocolSource : public InputSource {
private:
    vector<MessageType> fakeMessages {
            match_step,
            match_step,
            begin_scenario,
            invoke_step,
            end_scenario,
            match_step,
            match_step,
            begin_scenario,
            invoke_step,
            invoke_step,
            end_scenario
    };

protected:
    void processFakeMessages() {
        std::for_each(fakeMessages.begin(), fakeMessages.end(), [](MessageType & m) {
            switch (m) {
                case match_step:
                    matchStep();
                    break;
                case begin_scenario:
                    beginScenario();
                    break;
                case invoke_step:
                    invokeStep();
                    break;
                case end_scenario:
                    endScenario();
                    break;
            }
        });
    }
};

class TcpWireProtocolSource : public WireProtocolSource {
public:
    TcpWireProtocolSource(string host, int port) {
        cerr << "[DEBUG] TCP Wire Input " << host << ":" << port << endl;
    }

    unique_ptr<const Scenario> read() {
        cerr << "[DEBUG] Reading from TCP Wire" << endl;
        processFakeMessages();
        return unique_ptr<const Scenario>();
    };
};

class UnixWireProtocolSource : public WireProtocolSource {
public:
    UnixWireProtocolSource(string sock) {
        cerr << "[DEBUG] UNIX Wire Input " << sock << endl;
    }

    unique_ptr<const Scenario> read() {
        cerr << "[DEBUG] Reading from UNIX Wire" << endl;
        processFakeMessages();
        return unique_ptr<const Scenario>();
    };
};

static const regex protocolMatcher("^((tcp:([^:]+):)?(\\d+)|unix:([^:]+)|)$");


}

namespace cucumber {
namespace plugins {

/**
 * Plugin to support the wire protocol.
 *
 * This is a very special plugin because it acts like an input plugin but
 * it blocks on until the wire protocol has been consumed, and it returns
 * an empty input afterwards. This is because it is too low level and it
 * needs to interact directly with the engine.
 *
 * Other input and output plugins will still work as if this input source
 * hadn't been used, but they'll have to wait for this to be completely
 * consumed when the input source is read.
 *
 * Input format:
 *  - "" (default TCP port 3902)
 *  - "<port>"
 *  - "tcp:<hostname>:<port>"
 *  - "unix:<unix socket file>"
 *
 * Unsupported:
 *  - "tcp:<port>"
 *  - "<unix socket file>"
 */
class WireProtocolPlugin : public InputPlugin {
public:
    const char *name() const {
        return "wire";
    }

    unique_ptr<InputSource> inputFor(const string &expression) const {
        smatch matchResult;
        if (regex_search(expression, matchResult, protocolMatcher)) {
            string parsedHost = matchResult[3];
            string parsedPort = matchResult[4];
            string parsedSock = matchResult[5];
            if (parsedSock.empty()) {
                string host = parsedHost.empty() ? "localhost" : parsedHost;
                int port = parsedPort.empty() ? 3902 : stoi(parsedPort);
                return unique_ptr<InputSource>(new TcpWireProtocolSource(host, port));
            } else {
                return unique_ptr<InputSource>(new UnixWireProtocolSource(parsedSock));
            }
        } else {
            throw std::runtime_error("Couldn't parse wire input expression");
        }
    };
};

WireProtocolPlugin *wireProtocolPlugin() noexcept {
    static WireProtocolPlugin plugin;
    return &plugin;
}

BOOST_DLL_ALIAS_SECTIONED(wireProtocolPlugin, thisNeedsToBeUnique1, CukePlIn)

}
}


