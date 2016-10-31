#ifndef __MILAN_CORE_BINNEDLIKELIHOOD_H__
#define __MILAN_CORE_BINNEDLIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Types.hh"

#include "vdt/vdtMath.h"

namespace milan
{

class BinnedLikelihood:
    public LikelihoodInterface,
    public PtrInterface<LikelihoodInterface,BinnedLikelihood>
{
    protected:
        const Ptr<const HistogramInterface> _data;
        const Ptr<const HistogramInterface> _prediction;
        
        std::vector<Ptr<Parameter>> _bbParameters;
        
        const std::string _bbPrefix;

    public:
        BinnedLikelihood(const HistogramFunction& data, const HistogramFunction& prediction):
            _data(data.getPtr()),
            _prediction(prediction.getPtr()),
            _bbParameters(data.size()),
            _bbPrefix(std::to_string((sizetype)this)+"@bb")
        {
            if (data.getBinningVector()!=prediction.getBinningVector())
            {
                milan_throw("Cannot build BinnedLikelhood: Data and prediction have different binnings");
            }
            for (sizetype i = 0; i < _bbParameters.size(); ++i)
            {
                //BB parameters restricted to -5 .. 5 with 0.02 as step width; use object's address for unique name
                _bbParameters[i]=Ptr<Parameter>(PtrStorage::OWN,new Parameter(_bbPrefix+std::to_string(i+1),0.0,-5,5,0.02));
            }
        }
        
        inline const Ptr<const HistogramInterface>& getData() const
        {
            return _data;
        }
        
        inline const Ptr<const HistogramInterface>& getPrediction() const
        {
            return _prediction;
        }
        
        virtual std::vector<Ptr<Parameter>> getLagrangeParameters() const
        {
            return _bbParameters;
        }
        
        virtual double getNLL() const
        {
            const sizetype N = _prediction->size();
            double nll = 0;
            
            for (sizetype i = 0; i < N; ++i)
            {
                const double data = _data->getContent(i);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
            
                const double raw_prediction = _prediction->getContent(i); //no BB variation
                const double error = std::max(std::numeric_limits<double>::epsilon(),std::sqrt(_prediction->getError2(i)));
                
                if (raw_prediction<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
                
                const double bbValue = _bbParameters[i]->getValue();
                const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
                if (prediction<0)
                {
                    //cut off negative prediction, use infinity here since difference of finite values may be small again
                    return std::numeric_limits<double>::infinity();
                }

                nll+=data*vdt::fast_log(prediction)-prediction;
                nll+=0.5*bbValue*bbValue; //Gaussian prior with width=1
            }
            return nll;
        }
        
        virtual double getNLLDerivative(const Parameter& p) const
        {
            const sizetype N = _prediction->size();
            double diff_nll = 0;
            
            for (sizetype i = 0; i < N; ++i)
            {
                const double data = _data->getContent(i);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
                
                if (p.getName().find(_bbPrefix)!=std::string::npos)
                {
                    if (p==*_bbParameters[i])
                    {
                        const double error = std::max(std::numeric_limits<double>::epsilon(),std::sqrt(_prediction->getError2(i)));
                        const double raw_prediction = _prediction->getContent(i); //no BB variation
                        const double bbValue = _bbParameters[i]->getValue();
                        const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
                        if (prediction<0)
                        {
                            //cut off negative prediction, use infinity here since difference of finite values may be small again
                            return std::numeric_limits<double>::infinity();
                        }
                        diff_nll+=data*error/(1.+bbValue*error)-error*raw_prediction+bbValue;
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    const double error = std::max(std::numeric_limits<double>::epsilon(),std::sqrt(_prediction->getError2(i)));
                    const double raw_prediction = _prediction->getContent(i); //no BB variation
                    const double raw_prediction_diff = _prediction->getDifferential(i,p);
                    const double bbValue = _bbParameters[i]->getValue();
                    const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
                    if (prediction<0)
                    {
                        //cut off negative prediction, use infinity here since difference of finite values may be small again
                        return std::numeric_limits<double>::infinity();
                    }
                    
                    diff_nll+=(-1-bbValue*error)*raw_prediction_diff+data*raw_prediction_diff/raw_prediction;
                }
            }
            return diff_nll;
        }
        
        inline operator Likelihood() const
        {
            return copy();
        }
};

}

#endif
