#include "Plugin.hpp"

#include <iostream>
#include <regex>

using namespace cucumber;


class TcpWireProtocolSource : public InputSource {
public:
    TcpWireProtocolSource(string host, int port) {
        // TODO
        cerr << "[DEBUG] TCP Wire Input " << host << ":" << port << endl;
    }

    unique_ptr<const Scenario> read() {
        cerr << "[DEBUG] Reading from TCP Wire" << endl;
        return unique_ptr<const Scenario>();
    };
};

class UnixWireProtocolSource : public InputSource {
public:
    UnixWireProtocolSource(string sock) {
        // TODO
        cerr << "[DEBUG] UNIX Wire Input " << sock << endl;
    }

    unique_ptr<const Scenario> read() {
        cerr << "[DEBUG] Reading from UNIX Wire" << endl;
        return unique_ptr<const Scenario>();
    };
};

static const regex protocolMatcher("^((tcp:([^:]+):)?(\\d+)|unix:([^:]+)|)$");
// with this anything not number is unix, but that means it would take priority over feature files
//static const regex protocolMatcher("^((tcp:([^:]+):)?(\\d+)|(unix:)?([^:]+)|)$");

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
