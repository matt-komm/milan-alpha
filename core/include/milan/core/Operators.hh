#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

//TODO: eventually make an OperatorInterface
template<class INTERFACE, class RESULT>
class AddOperator:
    public INTERFACE
{
    protected:
        const Ptr<const INTERFACE> _lhs;
        const Ptr<const INTERFACE> _rhs;
        
    public:
        AddOperator(const Ptr<const INTERFACE>& lhs, const Ptr<const INTERFACE>& rhs):
            _lhs(lhs),
            _rhs(rhs)
        {
            //TODO: check compatibility (e.g. binning for Histograms) here
        }
        
        virtual RESULT get() const
        {
            RESULT hist = _lhs.get()->get();
            hist.add(_rhs.get()->get());
            return hist;
        }
};



//typedef AddOperator<HistogramInterface<1>,Histogram<1>> AddHist;


}
/*
template<class BASE>
BASE operator+(const BASE& lhs, const BASE& rhs)
{
    return milan::AddOperator<BASE,Histogram<1>>(lhs, rhs);
}
*/


#endif
