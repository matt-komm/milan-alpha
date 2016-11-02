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
        
        virtual const std::vector<Ptr<Parameter>>& getLagrangeParameters() const
        {
            return _llInterface->getLagrangeParameters();
        }
        
        virtual double getNLL() const
        {
            return _llInterface->getNLL();
        }
        
        virtual double getNLLDerivative(const Parameter& p) const
        {
            return _llInterface->getNLLDerivative(p);
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            return _llInterface->getNLLValueAndDerivatives(parameters);
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
