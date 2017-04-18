#include "Plugin.hpp"

#include <iostream>
#include <regex>

using namespace cucumber;

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

class WireProtocolPlugin : public NullPlugin {
public:

    const char *name() {
        return "wire";
    }

    unique_ptr<InputSource> inputFor(const string & expression) const {
        smatch matchResult;
        if (regex_search(expression, matchResult, protocolMatcher)) {
            string parsedHost = matchResult[3];
            string parsedPort = matchResult[4];
            string parsedSock = matchResult[6];
            if (parsedSock.empty()) {
                string host = parsedHost.empty() ? "localhost" : parsedHost;
                int port = parsedPort.empty() ? 3902 : stoi(parsedPort);
                return unique_ptr<InputSource>(new TcpWireProtocolSource(host, port));
            } else {
                return unique_ptr<InputSource>(new UnixWireProtocolSource(parsedSock));
            }
        } else {
            throw "Couldn't parse wire input expression";
        }
    };

    static const bool created;
};

const bool WireProtocolPlugin::created = registerPlugin<WireProtocolPlugin>();
