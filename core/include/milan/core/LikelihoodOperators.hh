#ifndef __MILAN_CORE_LIKELIHOODOPERATORS_H__
#define __MILAN_CORE_LIKELIHOODOPERATORS_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class MultiplyLikelihoodOperator:
    public LikelihoodInterface
{
    protected:
        Ptr<const LikelihoodInterface> _lhs;
        Ptr<const LikelihoodInterface> _rhs;
        
        std::vector<Ptr<Parameter>> _lagrangeParameters;
        
    public:
        MultiplyLikelihoodOperator(const Ptr<const LikelihoodInterface>& lhs,const Ptr<const LikelihoodInterface>& rhs):
            _lhs(lhs),
            _rhs(rhs)
        {
            std::copy(_lhs->getLagrangeParameters().begin(),_lhs->getLagrangeParameters().end(),std::back_inserter(_lagrangeParameters));
            std::copy(_rhs->getLagrangeParameters().begin(),_rhs->getLagrangeParameters().end(),std::back_inserter(_lagrangeParameters));
        }
        
        virtual const std::vector<Ptr<Parameter>>& getLagrangeParameters() const
        {            
            return _lagrangeParameters;
        }
        
        virtual double getNLL() const
        {
            return _lhs->getNLL()+_rhs->getNLL();
        }
        
        virtual double getNLLDerivative(const Ptr<Parameter>& parameter) const
        {
            return _lhs->getNLLDerivative(parameter)+_rhs->getNLLDerivative(parameter);
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> resultLhs(_lhs->getNLLValueAndDerivatives(parameters));
            std::vector<double> resultRhs(_rhs->getNLLValueAndDerivatives(parameters));
            for (sizetype i = 0; i < resultLhs.size(); ++i)
            {
                resultLhs[i]+=resultRhs[i];
            }
            return resultLhs;
        }
};

}

#endif
