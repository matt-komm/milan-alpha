#ifndef __MILAN_CORE_PARAMETER_H__
#define __MILAN_CORE_PARAMETER_H__

#include "milan/core/Types.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/FunctionInterface.hh"

#include <string>
#include <limits>
#include <algorithm>

namespace milan
{

class Parameter:
    public FunctionInterface,
    public PtrInterface<FunctionInterface,Parameter>
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
        
        inline bool operator==(const Parameter& p) const
        {
            return _name==p._name;
        }
        
        inline const std::string& getName() const
        {
            return _name;
        }
        
        virtual double getValue() const
        {
            return _value;
        }
        
        virtual double getDifferential(const Ptr<Parameter>& parameter) const
        {
            return *parameter==*this?1:0;
        }
        
        operator const Ptr<Parameter>()
        {
            return Ptr<Parameter>(PtrStorage::SHARE,this);
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> result(parameters.size()+1,0);
            result[0]=_value;
            std::transform(
                parameters.begin(),
                parameters.end(),
                result.begin(), 
                [this](const Ptr<Parameter>& p)
                { 
                    return this->getDifferential(p);
                }
            );
            return result;
        }
};

}

#endif
