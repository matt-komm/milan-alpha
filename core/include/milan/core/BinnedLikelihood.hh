#ifndef __MILAN_CORE_BINNEDLIKELIHOOD_H__
#define __MILAN_CORE_BINNEDLIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Types.hh"

namespace milan
{

class BinnedLikelihood:
    public LikelihoodInterface
{
    protected:
        const Ptr<const HistogramInterface> _data;
        const Ptr<const HistogramInterface> _prediction;
    public:
        BinnedLikelihood(const Ptr<const HistogramInterface>& data, const Ptr<const HistogramInterface>& prediction):
            _data(data),
            _prediction(prediction)
        {
            //TODO: check binning!
        }
};

}

#endif
