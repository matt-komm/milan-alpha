#ifndef __MILAN_CORE_LIKELIHOOD_H__
#define __MILAN_CORE_LIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class Likelihood:
    LikelihoodInterface
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
        
        //Likelihood operator*(const Likelihood& rhs) const;
};

}

#endif
