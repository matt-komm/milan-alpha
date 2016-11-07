#ifndef __MILAN_ALGORITHMS_HMC_H__
#define __MILAN_ALGORITHMS_HMC_H__

#include "milan/core/Types.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/LikelihoodInterface.hh"

#include "milan/algorithms/RandomEngine.hh"

#include "vdt/vdtMath.h"

#include <vector>
#include <random>

namespace milan
{

class HMC
{
    protected:

        std::vector<Ptr<Parameter>> _parameters;
        std::normal_distribution<double> _normalDistribution;
        std::uniform_real_distribution<double> _uniformDistribution;
        RandomEngine _randomEngine;
        //std::default_random_engine _randomEngine;
        //std::mt19937_64 _randomEngine;
    public:
        HMC(sizetype seed=123):
            _normalDistribution(0,1),
            _uniformDistribution(0,1),
            _randomEngine(seed)
        {
        }
        
        inline const std::vector<Ptr<Parameter>>& getParameters() const
        {
            return _parameters;
        }
        
        inline void addParameter(Parameter& parameter)
        {
            addParameter(Ptr<Parameter>(PtrStorage::SHARE,&parameter));
        }
        
        inline void addParameter(const Ptr<Parameter>& parameter)
        {
            for (sizetype i = 0; i < _parameters.size(); ++i)
            {
                if (*_parameters[i]==*parameter)
                {
                    milan_throw("Attempt to add same parameter with name '",parameter->getName(),"' twice for HMC");
                }
            }
            _parameters.push_back(parameter);
        }
        
        inline void optimizeStepsSize(const Ptr<LikelihoodInterface>&, sizetype)
        {
            //try to reach ~80% acceptance after burn in
        }
        
        inline sizetype optimizeSubStepNumber(const Ptr<LikelihoodInterface>&)
        {
            return 0;
        }
        
        bool step(const Ptr<const LikelihoodInterface>& ll, sizetype L)
        {
            const sizetype N = _parameters.size();
            std::vector<double> startX(N);
            std::vector<double> endX(N);
            
            std::vector<double> startP(N);
            std::vector<double> endP(N);
            
            std::vector<double> eps(N);
            
            double oldEnergy = 0.0;
            double newEnergy = 0.0;
            
            std::vector<double> valueAndGradient = ll->getNLLValueAndDerivatives(_parameters);
            oldEnergy += valueAndGradient[0];
            
            for (sizetype i = 0; i < N; ++i)
            {
                startX[i] = _parameters[i]->getValue();
                startP[i] = _normalDistribution(_randomEngine);
                
                endX[i] = startX[i];
                endP[i] = startP[i];
                
                eps[i] = _parameters[i]->getStep();
                
                oldEnergy += 0.5*startP[i]*startP[i];
            }


            
            //half step in p to start leap frog
            for (sizetype i = 0; i < N; ++i)
            {
                endP[i]=endP[i] - 0.5*eps[i]*valueAndGradient[i+1];
            }
            //leap frogging L-1 times
            for (sizetype i = 0; i < L; ++i)
            {   
                for (sizetype i = 0; i < N; ++i)
                {
                    endX[i] = endX[i] + eps[i]*endP[i];
                    _parameters[i]->setValue(endX[i]);
                }
                valueAndGradient = ll->getNLLValueAndDerivatives(_parameters);
                if (i!=(L-1))
                {
                    for (sizetype i = 0; i < N; ++i)
                    {
                        endP[i] = endP[i] - eps[i]*valueAndGradient[i+1];
                    }
                }
            }
            newEnergy+=valueAndGradient[0];
            //half step in p to finish leap frog
            for (sizetype i = 0; i < N; ++i)
            {
                endP[i] -= 0.5*eps[i]*valueAndGradient[i+1];
                newEnergy+=0.5*endP[i]*endP[i];
            }

            //reset parameters if not accepted
            if (std::isnan(newEnergy) or _uniformDistribution(_randomEngine)>vdt::fast_exp(oldEnergy-newEnergy))
            {
                for (sizetype i = 0; i < N; ++i)
                {
                    _parameters[i]->setValue(startX[i]);
                }
                return false;
            }
            return true;
        }
   
};

}

#endif
