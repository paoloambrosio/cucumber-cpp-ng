#include "extensions.hpp"

#include <boost/dll.hpp>

using namespace std;
namespace dll = boost::dll;
namespace filesystem = boost::filesystem;

static vector<shared_ptr<dll::shared_library>> loadedLibs; // it must be outside to be unloaded at the end

void cucumber::loadExtension(const filesystem::path path) {
    auto lib = make_shared<dll::shared_library>(path);
    loadedLibs.push_back(lib);
}
