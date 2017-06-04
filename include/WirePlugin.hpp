#ifndef CUCUMBER_CPP_WIREPLUGIN_H
#define CUCUMBER_CPP_WIREPLUGIN_H

#include <Plugin.hpp>

namespace cucumber {

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
    const char *name() const;
    unique_ptr<InputSource> inputFor(const string &expression) const;
//private:
//    static const bool inputRegistered;
};

}

#endif //CUCUMBER_CPP_WIREPLUGIN_H
