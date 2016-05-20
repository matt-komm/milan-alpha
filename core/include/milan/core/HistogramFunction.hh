#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Operators.hh"
#include "milan/core/Ptr.hh"

#include <iostream>
#include <memory>

namespace milan
{

template<sizetype DIM>
class HistogramFunction:
    public HistogramInterface<DIM>
{
    public:
        typedef HistogramInterface<DIM> Inteface;
    protected:
        Ptr<const HistogramInterface<DIM>> _histFct;
    public:
        HistogramFunction<DIM>(const HistogramFunction<DIM>& hist):
            _histFct(hist._histFct)
        {
        }
        
        HistogramFunction<DIM>(HistogramFunction<DIM>&& hist):
            _histFct(std::move(hist._histFct))
        {
        }
        
        HistogramFunction<DIM>& operator=(const HistogramFunction<DIM>& hist)
        {
            _histFct = hist._histFct;
            return *this;
        }
        /*
        HistogramFunction<DIM> operator=(HistogramFunction<DIM>&& hist)
        {
            _histFct = std::move(hist._histFct);
            return *this;
        }
        */
        HistogramFunction<DIM>(const Ptr<const HistogramInterface<DIM>>& hist):
            _histFct(hist)
        {
        }
        
        virtual Histogram<DIM> get() const
        {
            return _histFct.get()->get();
        }
        
        HistogramFunction<DIM> operator+(const HistogramFunction<DIM>& rhs) const;
};

template<sizetype DIM>
HistogramFunction<DIM> HistogramFunction<DIM>::operator+(const HistogramFunction<DIM>& rhs) const
{
    AddOperator<HistogramInterface<DIM>,Histogram<DIM>>* op = new AddOperator<HistogramInterface<DIM>,Histogram<DIM>>(
        this->_histFct,
        rhs._histFct
    );
    Ptr<const HistogramInterface<DIM>> res(PtrStorage::OWN,op);
    return res;
}




}


#endif
