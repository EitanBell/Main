#define I_AM_THE_HANDLETON_IMPLEMENTER
#include "handleton.hpp"    // Handleton
#include "plugin_main.hpp"
#include "factory_singleton.hpp"

using namespace ilrd;

typedef FactorySingleton<PluginBase, std::string> T;
INSTANTIATE_HANDLETON(T)