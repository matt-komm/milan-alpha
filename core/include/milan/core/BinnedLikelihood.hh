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
                
                const double bbValue = _bbParameters[i]->getValue();
                const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
                
                if (prediction<0)
                {
                    //cut off negative prediction, use infinity here since difference of finite values may be small again
                    return std::numeric_limits<double>::quiet_NaN();
                }
                
                nll+=data*vdt::fast_log(prediction)-prediction;
                nll+=0.5*bbValue*bbValue; //Gaussian prior with width=1
                
                if (std::isinf(nll) || std::isnan(nll))
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }
            }
            return nll;
        }
        
        virtual double getNLLDerivative(const Parameter& p) const
        {
            const sizetype N = _prediction->size();
            double diff_nll = 0;
            
            //todo swap loop with name if
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
                            return std::numeric_limits<double>::quiet_NaN();
                        }
                        
                        diff_nll+=data*error/(1.+bbValue*error)-error*raw_prediction+bbValue;
                        
                        if (std::isinf(diff_nll) || std::isnan(diff_nll)) 
                        {
                            return std::numeric_limits<double>::quiet_NaN();
                        }
                    }
                }
                else
                {
                    //TODO what if bb parameter but not from this ll?
                    const double error = std::max(std::numeric_limits<double>::epsilon(),std::sqrt(_prediction->getError2(i)));
                    const double raw_prediction = _prediction->getContent(i); //no BB variation
                    const double bbValue = _bbParameters[i]->getValue();
                    const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
                    
                    if (prediction<0)
                    {
                        //cut off negative prediction, use infinity here since difference of finite values may be small again
                        return std::numeric_limits<double>::quiet_NaN();
                    }
                    
                    const double raw_prediction_diff = _prediction->getDifferential(i,p);
                    
                    diff_nll+=(-1-bbValue*error)*raw_prediction_diff+data*raw_prediction_diff/raw_prediction;
                    
                    if (std::isinf(diff_nll) || std::isnan(diff_nll))
                    {
                        return std::numeric_limits<double>::quiet_NaN();
                    }
                }
            }
            return diff_nll;
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Parameter>& parameters) const
        {
            std::vector<double> result(parameters.size()+1,0);
            const sizetype N = _prediction->size();
            
            
            for (sizetype ibin = 0; ibin < N; ++ibin)
            {
                const double data = _data->getContent(ibin);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
            
                const double raw_prediction = _prediction->getContent(ibin); //no BB variation
                const double error = std::max(std::numeric_limits<double>::epsilon(),std::sqrt(_prediction->getError2(ibin)));
                
                const double bbValue = _bbParameters[ibin]->getValue();
                const double prediction = (1.+bbValue*error)*raw_prediction; //with BB variation
  
                if (prediction<0)
                {
                    //cut off negative prediction, use infinity here since difference of finite values may be small again
                    for (sizetype i = 0; i < result.size()+1; ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                    return result;
                } 
                
                result[0]+=data*vdt::fast_log(prediction)-prediction;
                result[0]+=0.5*bbValue*bbValue; //Gaussian prior with width=1
                
                if (std::isinf(result[0]) || std::isnan(result[0]))
                {
                    for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                    return result;
                }
  
                for (sizetype iparameter = 0; iparameter < parameters.size(); ++iparameter)
                {
                    const Parameter& p = parameters[iparameter];
                    if (p.getName().find(_bbPrefix)!=std::string::npos)
                    {
                        if (p==*_bbParameters[ibin])
                        {
                            result[iparameter+1]+=data*error/(1.+bbValue*error)-error*raw_prediction+bbValue;
                            if (std::isinf(result[iparameter+1]) || std::isnan(result[iparameter+1]))
                            {
                                for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                                return result;
                            }
                        }
                    }
                    else
                    {
                        const double raw_prediction_diff = _prediction->getDifferential(ibin,p);
                        result[iparameter+1]+=(-1-bbValue*error)*raw_prediction_diff+data*raw_prediction_diff/raw_prediction;
                        if (std::isinf(result[iparameter+1]) || std::isnan(result[iparameter+1]))
                        {
                            for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                            return result;
                        }
                    }
                }
            }
            return result;
        }
        
        inline operator Likelihood() const
        {
            return copy();
        }
        
        ~BinnedLikelihood()
        {

            
        }
};

}

#endif
