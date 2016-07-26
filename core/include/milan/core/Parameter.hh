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
        constexpr static double MIN = std::numeric_limits<float64>::lowest();
        constexpr static double MAX = std::numeric_limits<float64>::max();
    protected:
        const std::string _name;
        double _value;
        double _min;
        double _max;
        double _step;
        
    public:
        Parameter(std::string name, double value=0, double min=MIN, double max=MAX, double step = 0.1):
            _name(name),
            _value(value),
            _min(min),
            _max(max),
            _step(step)
        {
        }
        
        inline double getValue() const
        {
            return _value;
        }
        
        inline void setValue(double value)
        {
            _value = value;
        }
        
        inline double getStep() const
        {
            return _step;
        }
        
        inline void setStep(double step)
        {
            _step = step;
        } 
        
        bool operator==(const Parameter& p) const
        {
            return _name==p._name;
        }
};

}

#endif
