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
        
        
        virtual double getNLL() const
        {
            return _lhs->getNLL()+_rhs->getNLL();
        }
};

}

#endif
