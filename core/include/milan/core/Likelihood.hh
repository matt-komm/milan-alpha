#ifndef __MILAN_CORE_LIKELIHOOD_H__
#define __MILAN_CORE_LIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/LikelihoodOperators.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class Likelihood:
    public LikelihoodInterface
{
    protected:
        Ptr<const LikelihoodInterface> _llInterface;
    public:
        Likelihood(const Likelihood& ll):
            _llInterface(ll._llInterface)
        {
        }
        Likelihood(Likelihood&& ll):
            _llInterface(std::move(ll._llInterface))
        {
        }
        
        Likelihood& operator=(Likelihood&& ll)
        {
            _llInterface = std::move(ll._llInterface);
            return *this;
        }
        
        Likelihood& operator=(const Likelihood& ll)
        {
            _llInterface = ll._llInterface;
            return *this;
        }
        
        Likelihood(const Ptr<const LikelihoodInterface>& llInterface):
            _llInterface(llInterface)
        {
        }
        
        Likelihood& operator=(const Ptr<const LikelihoodInterface>& llInterface)
        {
            _llInterface = llInterface;
            return *this;
        }
        
        virtual std::vector<Ptr<Parameter>> getLagrangeParameters() const
        {
            return _llInterface->getLagrangeParameters();
        }
        
        virtual double getNLLValue() const
        {
            return _llInterface->getNLLValue();
        }
        
        virtual Ftype getNLLFtype() const
        {
            return _llInterface->getNLLFtype();
        }
        
        Likelihood operator*(const Likelihood& rhs) const;
        
        
};

Likelihood Likelihood::operator*(const Likelihood& rhs) const
{
    MultiplyLikelihoodOperator* op = new MultiplyLikelihoodOperator(
        this->_llInterface,
        rhs._llInterface
    );
    Ptr<const LikelihoodInterface> res(PtrStorage::OWN,op);
    return res;
}

}

#endif
