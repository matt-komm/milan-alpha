#ifndef __MILAN_OPERATORS_LIFTING_H__
#define __MILAN_OPERATORS_LIFTING_H__

#include "milan/core/Types.hh"
#include "milan/core/Histogram.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Exception.hh"

namespace milan
{

class Lifting: //will make a higher dimensional histogram given N lower ones using provided binning for the new axis
    public HistogramInterface
{
    protected:
        const Ptr<const HistogramInterface>& _src;
        const std::vector<sizetype>& _axes;
    public:
        Lifting(const Binning& binning, const std::vector<Ptr<const HistogramInterface>>& src):
            _src(src),
            _axes(axes)
        {
        }

        
        
};

}

#endif
