#ifndef __MILAN_CORE_PARAMETER_H__
#define __MILAN_CORE_PARAMETER_H__

#include "milan/core/Types.hh"

#include <string>
#include <limits>

namespace milan
{

class Parameter
{
    public:
        constexpr static float64 MIN = std::numeric_limits<float64>::lowest();
        constexpr static float64 MAX = std::numeric_limits<float64>::max();
    protected:
        const std::string _name;
        float64 _value;
        float64 _min;
        float64 _max;
    public:
        Parameter(std::string name, float64 value=0, float64 min=MIN, float64 max=MAX):
            _name(name),
            _value(value),
            _min(min),
            _max(max)
        {
        }
        
        inline float64 getValue() const
        {
            return _value;
        }
        
        inline void setValue(float64 value)
        {
            _value = value;
        }
};

}

#endif
