#ifndef __MILAN_CORE_FUNCTIONOPERATORS_H__
#define __MILAN_CORE_FUNCTIONOPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/Function.hh"
#include "milan/core/FunctionInterface.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class FunctionAddOperator:
    public FunctionInterface
{
    protected:
        const Ptr<const FunctionInterface> _lhs;
        const Ptr<const FunctionInterface> _rhs;
        
    public:
        FunctionAddOperator(
            const Ptr<const FunctionInterface>& lhs, 
            const Ptr<const FunctionInterface>& rhs
        ):
            _lhs(lhs),
            _rhs(rhs)
        {
        }
        
        virtual double getValue() const
        {
            return _lhs->getValue()+_rhs->getValue();
        }
        
        virtual double getDerivative(const Ptr<Parameter>& parameter) const
        {
            return _lhs->getDerivative(parameter)+_rhs->getDerivative(parameter);
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> resultLhs(_lhs->getValueAndDerivatives(parameters));
            std::vector<double> resultRhs(_rhs->getValueAndDerivatives(parameters));
            for (sizetype i = 0; i < resultLhs.size(); ++i)
            {
                resultLhs[i]+=resultRhs[i];
            }
            return resultLhs;
        }

};

}


#endif
