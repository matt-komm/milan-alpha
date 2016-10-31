#ifndef __MILAN_OPERATORS_PROJECTIONS_H__
#define __MILAN_OPERATORS_PROJECTIONS_H__

#include "milan/core/Types.hh"
#include "milan/core/Histogram.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Exception.hh"

namespace milan
{

class Projection:
    public HistogramInterface
{
    protected:
        const Ptr<const HistogramInterface>& _src;
        const std::vector<sizetype>& _axes;
    public:
        Projection(const Ptr<const HistogramInterface>& src, const std::vector<sizetype>& axes):
            _src(src),
            _axes(axes)
        {
        }

        
        
};

}

#endif
