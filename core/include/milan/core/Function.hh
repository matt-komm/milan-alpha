#ifndef __MILAN_CORE_FUNCTION_H__
#define __MILAN_CORE_FUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Parameter.hh"

#include <string>

namespace milan
{

class Function:
    public FunctionInterface
{
    protected:
        Ptr<const FunctionInterface> _function;
    public:
        virtual double getValue() const
        {
            return _function->getValue();
        }
        
        virtual double getDifferential(const Ptr<Parameter>& parameter) const
        {
            return _function->getDifferential(parameter);
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            return _function->getValueAndDerivatives(parameters);
        }
};

}

#endif
