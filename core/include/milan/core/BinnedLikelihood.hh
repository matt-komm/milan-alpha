#ifndef __MILAN_CORE_BINNEDLIKELIHOOD_H__
#define __MILAN_CORE_BINNEDLIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Likelihood.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Types.hh"

#include "milan/core/Math.hh"

namespace milan
{

class BinnedLikelihood:
    public LikelihoodInterface,
    public PtrInterface<LikelihoodInterface,BinnedLikelihood>
{
    protected:
        const Ptr<const HistogramInterface> _data;
        const Ptr<const HistogramInterface> _template;
        
        std::vector<Ptr<Parameter>> _bbParameters;
        
        const std::string _bbPrefix;
        
    public:
        BinnedLikelihood(const HistogramFunction& data, const HistogramFunction& prediction):
            _data(data.getPtr()),
            _template(prediction.getPtr()),
            _bbParameters(data.size()),
            _bbPrefix(std::to_string((sizetype)this)+"bb@bin") 
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
            return _template;
        }
        
        virtual const std::vector<Ptr<Parameter>>& getLagrangeParameters() const
        {
            return _bbParameters;
        }
        
        virtual double getNLL() const
        {
            const sizetype N = _template->size();
            double nll = 0;
            
            for (sizetype ibin = 0; ibin < N; ++ibin)
            {
                const double data = _data->getContent(ibin);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
            
                const double raw_template = _template->getContent(ibin); //no BB variation
                const double error = std::sqrt(_template->getError2(ibin));
                
                const double bbValue = _bbParameters[ibin]->getValue();
                const double prediction = raw_template+bbValue*error; //with BB variation
                
                if (prediction<std::numeric_limits<double>::epsilon())
                {
                    //cut off negative prediction, use infinity here since difference of finite values may be small again
                    return std::numeric_limits<double>::quiet_NaN();
                }
                
                nll+=prediction-data*milan::log(prediction);
                nll+=0.5*bbValue*bbValue; //Gaussian prior with width=1
                
                if (std::isinf(nll) || std::isnan(nll))
                {
                    return std::numeric_limits<double>::quiet_NaN();
                }
            }
            return nll;
        }
        
        virtual double getNLLDerivative(const Ptr<Parameter>& parameter) const
        {
            const sizetype N = _template->size();
            double diff_nll = 0;
            
            //todo swap loop with name if
            for (sizetype ibin = 0; ibin < N; ++ibin)
            {
                const double data = _data->getContent(ibin);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
                
                if (parameter->getName().find(_bbPrefix)!=std::string::npos)
                {
                    if (*parameter==*_bbParameters[ibin])
                    {
                        const double error = std::sqrt(_template->getError2(ibin));
                        
                        const double raw_template = _template->getContent(ibin); //no BB variation
                        const double bbValue = _bbParameters[ibin]->getValue();
                        const double prediction = raw_template+bbValue*error; //with BB variation
                        
                        if (prediction<std::numeric_limits<double>::epsilon())
                        {
                            //cut off negative prediction, use infinity here since difference of finite values may be small again
                            return std::numeric_limits<double>::quiet_NaN();
                        }
                        
                        if (error<std::numeric_limits<double>::epsilon())
                        {
                            //deactivate BB if error == 0
                            continue;
                        }
                        diff_nll+=-data*error/prediction+error-bbValue;
                        
                        if (std::isinf(diff_nll) || std::isnan(diff_nll)) 
                        {
                            return std::numeric_limits<double>::quiet_NaN();
                        }
                    }
                }
                else
                {
                    
                    const double error = std::sqrt(_template->getError2(ibin));
                    const double raw_template = _template->getContent(ibin); //no BB variation
                    const double bbValue = _bbParameters[ibin]->getValue();
                    const double prediction = raw_template+bbValue*error; //with BB variation
                    
                    if (prediction<std::numeric_limits<double>::epsilon())
                    {
                        //cut off negative prediction, use infinity here since difference of finite values may be small again
                        return std::numeric_limits<double>::quiet_NaN();
                    }
                    
                    //this will be 0 if p is a bb parameter of a different ll
                    const double raw_template_diff = _template->getDerivative(ibin,parameter);
                    
                    diff_nll+=-data*raw_template_diff/prediction+raw_template_diff;
                    
                    if (std::isinf(diff_nll) || std::isnan(diff_nll))
                    {
                        return std::numeric_limits<double>::quiet_NaN();
                    }
                }
            }
            return diff_nll;
        }
        
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> result(parameters.size()+1,0);
            const sizetype N = _template->size();
            
            for (sizetype ibin = 0; ibin < N; ++ibin)
            {
                const double data = _data->getContent(ibin);
                if (data<std::numeric_limits<double>::epsilon())
                {
                    continue;
                }
            
                const double raw_template = _template->getContent(ibin); //no BB variation
                const double error = std::sqrt(_template->getError2(ibin));
                
                const double bbValue = _bbParameters[ibin]->getValue();
                const double prediction = raw_template+bbValue*error; //with BB variation
  
                if (prediction<std::numeric_limits<double>::epsilon())
                {
                    //cut off negative prediction, use infinity here since difference of finite values may be small again
                    for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                    return result;
                } 
                
                result[0]+=prediction-data*milan::log(prediction);
                result[0]+=0.5*bbValue*bbValue; //Gaussian prior with width=1
                
                if (std::isinf(result[0]) || std::isnan(result[0]))
                {
                    for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                    return result;
                }
  
                for (sizetype iparameter = 0; iparameter < parameters.size(); ++iparameter)
                {
                    const Ptr<Parameter>& parameter = parameters[iparameter];
                    if (parameter->getName().find(_bbPrefix)!=std::string::npos)
                    {
                        if (error<std::numeric_limits<double>::epsilon())
                        {
                            //deactivate BB if error == 0
                            continue;
                        }
                        if (*parameter==*_bbParameters[ibin])
                        {
                            result[iparameter+1]+=-data*error/prediction+error-bbValue;
                            if (std::isinf(result[iparameter+1]) || std::isnan(result[iparameter+1]))
                            {
                                for (sizetype i = 0; i < result.size(); ++i) result[i]=std::numeric_limits<double>::quiet_NaN();
                                return result;
                            }
                        }
                    }
                    else
                    {
                        const double raw_template_diff = _template->getDerivative(ibin,parameter);
                        result[iparameter+1]+=-data*raw_template_diff/prediction+raw_template_diff;
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
