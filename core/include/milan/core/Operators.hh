#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramFunction.hh"

namespace milan
{

//template<class


//TODO: make this more templatized so that one can also use it for others eg parameters or continious functions
template<class INTERFACE, class RESULT>
class AddOperator:
    public INTERFACE
{
    protected:
        const std::shared_ptr<const INTERFACE> _lhs;
        const std::shared_ptr<const INTERFACE> _rhs;
        
    public:
        AddOperator(const INTERFACE& lhs, const INTERFACE& rhs):
            _lhs(lhs.clone()),
            _rhs(rhs.clone())
        {
            //TODO: check binning here
        }
        
        virtual RESULT get() const
        {
            RESULT hist = _lhs->get();
            hist.add(_rhs->get());
            return hist;
        }
        
        virtual std::shared_ptr<const INTERFACE> clone() const
        {
            return std::make_shared<AddOperator<INTERFACE,RESULT>>(*_lhs,*_rhs);
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
