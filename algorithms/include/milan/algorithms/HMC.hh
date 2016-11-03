#ifndef __MILAN_ALGORITHMS_HMC_H__
#define __MILAN_ALGORITHMS_HMC_H__

#include "milan/core/Types.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/LikelihoodInterface.hh"

#include "milan/algorithms/RandomEngine.hh"

#include <vector>
#include <random>

namespace milan
{

class HMC
{
    protected:

        std::vector<Ptr<Parameter>> _parameters;
        std::normal_distribution<double> _normalDistribution;
        RandomEngine _randomEngine;
    public:
        HMC(sizetype seed=123):
            _normalDistribution(0,1),
            _randomEngine(seed)
        {
        }
        
        inline const std::vector<Ptr<Parameter>>& getParameters() const
        {
            return _parameters;
        }
        
        inline void addParameter(const Ptr<Parameter>& parameter)
        {
            for (sizetype i = 0; i < _parameters.size(); ++i)
            {
                if (*_parameters[i]==*parameter)
                {
                    milan_throw("Atempt to add same parameter with name '",parameter->getName(),"' twice for HMC");
                }
            }
            _parameters.push_back(parameter);
        }
        
        inline void optimizeStepsSize(const Ptr<LikelihoodInterface>& ll, sizetype L)
        {
            //try to reach ~80% acceptance after burn in
        }
        
        inline sizetype optimizeSubStepNumber(const Ptr<LikelihoodInterface>& ll)
        {
            return 0;
        }
        
        inline void step(const Ptr<LikelihoodInterface>& ll, sizetype L)
        {
            std::vector<double> startX(_parameters.size());
            std::vector<double> endX(_parameters.size());
            
            std::vector<double> startP(_parameters.size());
            std::vector<double> endP(_parameters.size());
            
            for (sizetype i = 0; i < _parameters.size(); ++i)
            {
                startX[i] = _parameters[i]->getValue();
                startP[i] = _normalDistribution(_randomEngine);
            }
            
            for (sizetype i = 0; i < L; ++i)
            {
                ll->getNLLValueAndDerivatives(_parameters);
            }
        }
   
};

}

#endif
