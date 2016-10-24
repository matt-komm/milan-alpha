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
                _bbParameters[i]=Ptr<Parameter>(PtrStorage::OWN,new Parameter("bb"+std::to_string(i+1),0.0));
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
                const double prediction = _prediction->getContent(i);
                const double data = _data->getContent(i);
                //TODO: check correct equation!!!
                nll+=(data*vdt::fast_log(prediction)-prediction+_bbParameters[i]->getValue()*prediction);
            }
            return nll;
        }
        
        inline operator Likelihood() const
        {
            return copy();
        }
};

}

#endif
