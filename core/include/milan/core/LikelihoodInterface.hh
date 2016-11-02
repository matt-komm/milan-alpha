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
        virtual double getNLLDerivative(const Parameter& p) const = 0;
        virtual double getNLL() const = 0;
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const = 0;
        
        //returns pointers to potential lagrange parameters (e.g. for Barlow-Beeston) for additional minimizations
        virtual const std::vector<Ptr<Parameter>>& getLagrangeParameters() const = 0;
        
        virtual ~LikelihoodInterface()
        {
        }
        
};

}

#endif
