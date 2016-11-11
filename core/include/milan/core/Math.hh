#ifndef __MILAN_CORE_MATH_H__
#define __MILAN_CORE_MATH_H__

#ifdef FAST_MATH
#include "vdt/vdtMath.h"
#endif

#include <cmath>
#include <limits>

namespace milan
{

    template<
        typename T,
        typename = typename std::enable_if<
            std::is_unsigned<T>::value and std::numeric_limits<T>::is_integer,T>::type
        > T rotl(const T& r, const T& n)
    {
        return (((r)<<(n)) | ((r)>>((8*sizeof(r))-(n))));
    }
    
    template<
        typename T,
        typename = typename std::enable_if<
            std::is_floating_point<T>::value,T>::type
        > T log(const T& x)
    {
        #ifdef FAST_MATH
        return vdt::fast_log(x);
        #else
        return std::log(x);
        #endif
    }
    
    template<> float32 log(const float32& x)
    {
        #ifdef FAST_MATH
        return vdt::fast_logf(x);
        #else
        return std::log(x);
        #endif
    }
    
    template<
        typename T,
        typename = typename std::enable_if<
            std::is_floating_point<T>::value,T>::type
        > T exp(const T& x)
    {
        #ifdef FAST_MATH
        return vdt::fast_exp(x);
        #else
        return std::exp(x);
        #endif
    }
    
    template<> float32 exp(const float32& x)
    {
        #ifdef FAST_MATH
        return vdt::fast_expf(x);
        #else
        return std::exp(x);
        #endif
    }
}

#endif
