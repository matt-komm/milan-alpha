#ifndef __MILAN_CORE_BINNEDLIKELIHOOD_H__
#define __MILAN_CORE_BINNEDLIKELIHOOD_H__

#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Types.hh"

namespace milan
{

template<sizetype DIM>
class BinnedLikelihood:
    public LikelihoodInterface
{
    protected:
        const Ptr<const HistogramInterface<DIM>> _data;
        const Ptr<const HistogramInterface<DIM>> _prediction;
    public:
        BinnedLikelihood(const Ptr<const HistogramInterface<DIM>>& data, const Ptr<const HistogramInterface<DIM>>& prediction):
            _data(data),
            _prediction(prediction)
        {
        }
};

}

#endif
