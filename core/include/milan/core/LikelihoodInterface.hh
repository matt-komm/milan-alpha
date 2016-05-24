#ifndef __MILAN_CORE_LIKELIHOODINTERFACE_H__
#define __MILAN_CORE_LIKELIHOODINTERFACE_H__

#include "milan/core/Parameter.hh"
#include "milan/core/Ptr.hh"

#include <vector>

namespace milan
{

class LikelihoodInterface
{
    protected:
        std::vector<Ptr<Parameter>> _parameters;
    public:
        virtual double getNLLDerivative(const Parameter& p) const = 0;
        virtual double getNLL() const = 0;
        
        inline void addParameter(Ptr<Parameter>& parameter)
        {
        }
        
        virtual ~LikelihoodInterface()
        {
        }
        
};

}

#endif
