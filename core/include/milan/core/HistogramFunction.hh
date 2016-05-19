#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Operators.hh"

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
        std::shared_ptr<const HistogramInterface<DIM>> _histFct;
    public:
        HistogramFunction<DIM>(const HistogramFunction<DIM>& hist):
            _histFct(hist._histFct)
        {
        }
        
        HistogramFunction<DIM>(HistogramFunction<DIM>&& hist):
            _histFct(std::move(hist._histFct))
        {
        }
        /*
        HistogramFunction<DIM>(HistogramInterface<DIM>&& hist):
            _histFct(hist.clone())
        {
        }
        
        HistogramFunction<DIM>(const HistogramInterface<DIM>& hist):
            _histFct(hist.clone())
        {
        }
        */
        

        HistogramFunction<DIM>(const std::shared_ptr<const HistogramInterface<DIM>>& hist):
            _histFct(hist)
        {
        }
        /*
        HistogramFunction<DIM>(const HistogramInterface<DIM>* hist):
            _histFct(hist)
        {
        }
        */
    
        virtual Histogram<DIM> get() const
        {
            return _histFct->get();
        }
        /*
        virtual std::shared_ptr<const HistogramInterface<DIM>> clone() const
        {
            return std::make_shared<const HistogramFunction<DIM>>(_histFct);
        }
        */
};

template<sizetype DIM>
HistogramFunction<DIM> operator+(const HistogramInterface<DIM>& lhs, const HistogramInterface<DIM>& rhs)
{
    std::shared_ptr<const HistogramInterface<DIM>> res = std::make_shared<AddOperator<HistogramInterface<1>,Histogram<1>>>(lhs,rhs);
    return res;
}

}


#endif
