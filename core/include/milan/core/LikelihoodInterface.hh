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
        //virtual double getNLLDerivative(const Parameter& p) const = 0;
        virtual double getNLL() const = 0;
        
        /*
        inline void addParameter(Ptr<Parameter>& parameter)
        {
        }
        */
        
        //returns pointers to potential lagrange parameters (e.g. for Barlow-Beeston) for additional minimizations
        virtual std::vector<Ptr<Parameter>> getLagrangeParameters() const
        {
            return std::vector<Ptr<Parameter>>(0);
        }
        
        virtual ~LikelihoodInterface()
        {
        }
        
};

}

#endif
