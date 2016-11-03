#ifndef __MILAN_CORE_FUNCTIONINTERFACE_H__
#define __MILAN_CORE_FUNCTIONINTERFACE_H__

#include "milan/core/Types.hh"
#include "milan/core/Ptr.hh"

#include <vector>

namespace milan
{

class Parameter;

class FunctionInterface
{
    public:
        virtual double getValue() const = 0;
        virtual double getDifferential(const Ptr<Parameter>& parameter) const = 0;
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const = 0;

        virtual ~FunctionInterface()
        {
        }
};

}

#endif
