#ifndef ILRD_RD136_7_PLUGIN_MAIN_HPP
#define ILRD_RD136_7_PLUGIN_MAIN_HPP

#include <iostream>         // cout

#include "handleton.hpp"    // Handleton
#include "factory_singleton.hpp"

namespace ilrd
{

class PluginBase
{
public:
    PluginBase() = default;
    virtual ~PluginBase() = default;

    virtual void PluginPrint() {std::cout << "PluginBase Print" << std::endl;}
};

}   // namespace ilrd


#endif /* ILRD_RD136_7_PLUGIN_MAIN_HPP */