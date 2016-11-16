#ifndef __MILAN_CORE_PRIOR_H__
#define __MILAN_CORE_PRIOR_H__

#include "milan/core/Types.hh"
#include "milan/core/FunctionInterface.hh"
#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Ptr.hh"

#include "milan/core/Math.hh"

namespace milan
{

class Prior:
    public PtrInterface<LikelihoodInterface,Prior>,
    public PtrInterface<FunctionInterface,Prior>
{
    protected:
        const Ptr<FunctionInterface> _nllFct;
        
        const std::vector<Ptr<Parameter>> _lagrangianParameters;
    public:
        Prior(const Ptr<FunctionInterface> nllFct):
            _nllFct(nllFct)
        {
        }
        
        virtual double getValue() const
        {
            return milan::exp(-_nllFct->getValue());
        }
        
        virtual double getDerivative(const Ptr<Parameter>& parameter) const
        {
            return -_nllFct->getDerivative(parameter)*milan::exp(-_nllFct->getValue());
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> valueAndDiff = _nllFct->getValueAndDerivatives(parameters);
            valueAndDiff[0]=milan::exp(-valueAndDiff[0]);
            for (sizetype i = 0; i < parameters.size(); ++i)
            {
                valueAndDiff[i+1]=-valueAndDiff[i+1]*valueAndDiff[0];
            }
            return valueAndDiff;
        }

        virtual double getNLLDerivative(const Ptr<Parameter>& parameter) const
        {
            return _nllFct->getDerivative(parameter);
        }
        
        virtual double getNLL() const
        {
            return _nllFct->getValue();
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            return _nllFct->getValueAndDerivatives(parameters);
        }
        
        //returns pointers to potential lagrange parameters (e.g. for Barlow-Beeston) for additional minimizations
        virtual const std::vector<Ptr<Parameter>>& getLagrangeParameters() const
        {
            return _lagrangianParameters;
        }

};

}

#endif
