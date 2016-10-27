#ifndef __MILAN_CORE_BINNEDLIKELIHOOD_H__
#define __MILAN_CORE_BINNEDLIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/HistogramInterface.hh"
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
    public:
        BinnedLikelihood(const Ptr<const HistogramInterface>& data, const Ptr<const HistogramInterface>& prediction):
            _data(data),
            _prediction(prediction),
            _bbParameters(data->size())
        {
            if (data->getBinningVector()!=prediction->getBinningVector())
            {
                milan_throw("Cannot build BinnedLikelhood: Data and prediction have different binnings");
            }
            for (sizetype i = 0; i < _bbParameters.size(); ++i)
            {
                //BB parameters restricted to -5 .. 5 with 0.02 as step width; use object's address for unique name
                _bbParameters[i]=Ptr<Parameter>(PtrStorage::OWN,new Parameter(std::to_string((sizetype)this)+"@bb"+std::to_string(i+1),0.0,-5,5,0.02));
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
                const double error = std::sqrt(_prediction->getError2(i));
                
                if (raw_prediction<std::numeric_limits<double>::epsilon() and error<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
                
                
                const double prediction = (1+_bbParameters[i]->getValue()*error)*raw_prediction; //with BB variation
                
                nll+=data*vdt::fast_log(prediction)-prediction;
                nll+=0.5*(prediction-raw_prediction)*(prediction-raw_prediction); //Gaussian prior with width=1
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
                
                const double raw_prediction = _prediction->getContent(i); //no BB variation
                const double error = std::sqrt(_prediction->getError2(i));
                
                if (raw_prediction<std::numeric_limits<double>::epsilon() and error<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
                
                
                const double raw_prediction_diff = _prediction->getDifferential(i,p); //no BB variation
                
                const double prediction = (1+_bbParameters[i]->getValue()*error)*raw_prediction; //with BB variation
                
                if ((*_bbParameters[i])==p)
                {
                    diff_nll+=error*raw_prediction;
                }
                else
                {
                    diff_nll+=(1+_bbParameters[i]->getValue()*error)*raw_prediction_diff;
                }
                
                //const double raw_prediction = _prediction->getContent(i); //no BB variation
                //const double raw_prediction_diff = _prediction->getDifferential(p); //no BB variation
                
                
                //_bbParameters[i]->getDifferential(p)*error*raw_prediction+_bbParameters[i]->getValue()*error*raw_prediction_diff; //with BB variation
                
                

                diff_nll+=data*vdt::fast_log(prediction)-prediction;
                diff_nll+=0.5*(prediction-raw_prediction)*(prediction-raw_prediction); //Gaussian prior with width=1
            
            
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
