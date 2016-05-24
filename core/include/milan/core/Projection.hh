#ifndef __MILAN_CORE_PROJECTIONS_H__
#define __MILAN_CORE_PROJECTIONS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Exception.hh"

namespace milan
{

template<sizetype SOURCEDIM, sizetype TARGETDIM>
class Projection:
    public HistogramInterface<TARGETDIM>
{
    protected:
        const Ptr<const HistogramInterface<SOURCEDIM>>& _src;
        const std::vector<sizetype>& _axes;
    public:
        Projection(const Ptr<const HistogramInterface<SOURCEDIM>>& src, const std::vector<sizetype>& axes):
            _src(src),
            _axes(axes)
        {
            if (_axes.size()!=TARGETDIM)
            {
                milan_throw("Projected target dimension has to be the size of the projection axes: ",TARGETDIM,"=!",axes.size());
            }
        }
        
        virtual Histogram<TARGETDIM> getResult() const
        {
        }
        
        
};

}

#endif
