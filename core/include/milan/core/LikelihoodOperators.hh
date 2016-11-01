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
        
    public:
        MultiplyLikelihoodOperator(const Ptr<const LikelihoodInterface>& lhs,const Ptr<const LikelihoodInterface>& rhs):
            _lhs(lhs),
            _rhs(rhs)
        {
        }
        
        virtual std::vector<Ptr<Parameter>> getLagrangeParameters() const
        {
            std::vector<Ptr<Parameter>> parameters;
            std::copy(_lhs->getLagrangeParameters().begin(),_lhs->getLagrangeParameters().end(),std::back_inserter(parameters));
            std::copy(_rhs->getLagrangeParameters().begin(),_rhs->getLagrangeParameters().end(),std::back_inserter(parameters));
            return parameters;
        }
        
        virtual double getNLL() const
        {
            return _lhs->getNLL()+_rhs->getNLL();
        }
        
        virtual double getNLLDerivative(const Parameter& p) const
        {
            return _lhs->getNLLDerivative(p)+_rhs->getNLLDerivative(p);
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Parameter>& p) const
        {
            std::vector<double> resultLhs(_lhs->getNLLValueAndDerivatives(p));
            std::vector<double> resultRhs(_rhs->getNLLValueAndDerivatives(p));
            for (sizetype i = 0; i < resultLhs.size(); ++i)
            {
                resultLhs[i]+=resultRhs[i];
            }
            return resultLhs;
        }
};

}

#endif
