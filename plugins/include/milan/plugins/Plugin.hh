#ifndef __MILAN_PLUGINS_PLUGIN_H__
#define __MILAN_PLUGINS_PLUGIN_H__

#include "milan/core/HistogramInterface.hh"

#include "cpx/PluginProducer.hpp"

#include <string>
#include <typeinfo>

namespace milan
{

template<class BASE>
class Plugin:
    public BASE
{
    protected:
    public:
    
        static std::string getPluginName()
        {
            return std::string("Plugin")+std::string(typeid(BASE).name());
        }
        
        static cpx::Version getVersion()
        {
            return cpx::Version(1,0);
        }
        
        static std::string getDescription()
        {
            return std::string("Plugin")+std::string(typeid(BASE).name());
        }
};

typedef cpx::PluginProducer<Plugin<milan::HistogramInterface>,cpx::Signature<>> TestPluginProducer;

}

#endif
