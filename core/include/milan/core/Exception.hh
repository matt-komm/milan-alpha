#ifndef __MILAN_CORE_EXCEPTION_H__
#define __MILAN_CORE_EXCEPTION_H__

#include <stdexcept>
#include <sstream>


namespace milan
{
    template<class VALUE> static void expandArgs(std::stringstream& stream, VALUE value)
    {
        stream<<value;
    }

    template<class VALUE, class ...ARGS>  static void expandArgs(std::stringstream& stream, VALUE value, ARGS... args)
    {
        stream<<value;
        expandArgs(stream,args...);
    }
}


#define milan_throw(...) \
    std::stringstream ss; milan::expandArgs(ss,__VA_ARGS__); throw std::runtime_error(ss.str());


#endif

