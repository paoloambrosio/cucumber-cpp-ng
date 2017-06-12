#ifndef CUCUMBER_CPP_PLUGINMANAGER_H
#define CUCUMBER_CPP_PLUGINMANAGER_H

#include "plugins/plugins.hpp"

#include <boost/filesystem.hpp>

namespace cucumber {
namespace plugins {

void loadPlugins(const boost::filesystem::path path);

unique_ptr<InputSource> firstInputMatching(const string & expression);

InputPlugin & findInputPlugin(const string & name);
OutputPlugin & findOutputPlugin(const string & name);

}
}

#endif //CUCUMBER_CPP_PLUGINMANAGER_H
