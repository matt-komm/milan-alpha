#ifndef __MILAN_ALGORITHMS_HMC_H__
#define __MILAN_ALGORITHMS_HMC_H__

#include "milan/core/Types.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Math.hh"
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
            
            for (sizetype iparameter = 0; iparameter < N; ++iparameter)
            {
                startX[iparameter] = _parameters[iparameter]->getValue();
                startP[iparameter] = _normalDistribution(_randomEngine);
                
                endX[iparameter] = startX[iparameter];
                endP[iparameter] = startP[iparameter];
                
                eps[iparameter] = _parameters[iparameter]->getStep();
                
                oldEnergy += 0.5*startP[iparameter]*startP[iparameter];
            }


            
            //half step in p to start leap frog
            for (sizetype iparameter = 0; iparameter < N; ++iparameter)
            {
                endP[iparameter]=endP[iparameter] - 0.5*eps[iparameter]*valueAndGradient[iparameter+1];
            }
            //leap frogging L-1 times
            for (sizetype istep = 0; istep < L; ++istep)
            {   
                for (sizetype iparameter = 0; iparameter < N; ++iparameter)
                {
                    endX[iparameter] = endX[iparameter] + eps[iparameter]*endP[iparameter];
                    _parameters[iparameter]->setValue(endX[iparameter]);
                }
                valueAndGradient = ll->getNLLValueAndDerivatives(_parameters);
                if (istep!=(L-1))
                {
                    for (sizetype iparameter = 0; iparameter < N; ++iparameter)
                    {
                        endP[iparameter] = endP[iparameter] - eps[iparameter]*valueAndGradient[iparameter+1];
                    }
                }
            }
            newEnergy+=valueAndGradient[0];
            //half step in p to finish leap frog
            for (sizetype iparameter= 0; iparameter < N; ++iparameter)
            {
                endP[iparameter] -= 0.5*eps[iparameter]*valueAndGradient[iparameter+1];
                newEnergy+=0.5*endP[iparameter]*endP[iparameter];
            }

            //reset parameters if not accepted
            if (std::isnan(newEnergy) or std::isinf(newEnergy) or _uniformDistribution(_randomEngine)>milan::exp(oldEnergy-newEnergy))
            {
                for (sizetype iparameter = 0; iparameter < N; ++iparameter)
                {
                    _parameters[iparameter]->setValue(startX[iparameter]);
                }
                return false;
            }
            return true;
        }
   
};

}

#endif
