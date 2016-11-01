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
    public:
        virtual double getNLLValue() const = 0;
        virtual Ftype getNLLFtype() const = 0;
        
        //returns pointers to potential lagrange parameters (e.g. for Barlow-Beeston) for additional minimizations
        virtual std::vector<Ptr<Parameter>> getLagrangeParameters() const = 0;
        
        virtual ~LikelihoodInterface()
        {
        }
        
};

}

#endif
