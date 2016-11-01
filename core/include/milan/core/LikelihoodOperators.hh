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
        
        virtual double getNLLValue() const
        {
            return _lhs->getNLLValue()+_rhs->getNLLValue();
        }
        
        virtual Ftype getNLLFtype() const
        {
            return _lhs->getNLLFtype()+_rhs->getNLLFtype();
        }
        
};

}

#endif
