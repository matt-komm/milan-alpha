#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramFunction.hh"

namespace milan
{

//template<class


//TODO: make this more templatized so that one can also use it for others eg parameters or continious functions
template<sizetype DIM>
class AddHist:
    public HistogramInterface<DIM>
{
    protected:
        const std::shared_ptr<const HistogramInterface<DIM>> _lhs;
        const std::shared_ptr<const HistogramInterface<DIM>> _rhs;
        
    public:
        AddHist(const HistogramInterface<DIM>& lhs, const HistogramInterface<DIM>& rhs):
            _lhs(lhs.clone()),
            _rhs(rhs.clone())
        {
            //TODO: check binning here
        }
        
        virtual Histogram<DIM> get() const
        {
            Histogram<DIM> hist = _lhs->get();
            hist.add(_rhs->get());
            return hist;
        }
        
        virtual std::shared_ptr<const HistogramInterface<DIM>> clone() const
        {
            return std::make_shared<AddHist<DIM>>(*_lhs,*_rhs);
        }
};

template<sizetype DIM>
HistogramFunction<DIM> HistogramFunction<DIM>::operator+(const HistogramInterface<DIM>& histFct) const
{
    return AddHist<DIM>(*this, histFct);
}

}


#endif
