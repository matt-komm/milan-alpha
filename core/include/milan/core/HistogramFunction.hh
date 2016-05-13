#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/Histogram.hh"

#include <iostream>
#include <memory>

namespace milan
{

template<sizetype DIM> class Histogram;


template<sizetype DIM>
class HistogramInterface
{
    protected:
    public:
        virtual Histogram<DIM> get() const = 0;
        virtual std::shared_ptr<HistogramInterface<DIM>> clone() const = 0;
};




template<sizetype DIM>
class HistogramFunction:
    public HistogramInterface<DIM>
{
    protected:
        std::shared_ptr<const HistogramInterface<DIM>> _histFct;
    public:
        HistogramFunction<DIM>(HistogramInterface<DIM>&& hist):
            _histFct(hist.clone())
        {
        }
        
        HistogramFunction<DIM>(const HistogramInterface<DIM>& hist):
            _histFct(hist.clone())
        {
        }
    
        virtual Histogram<DIM> get() const
        {
            return _histFct->get();
        }
        
        virtual std::shared_ptr<HistogramInterface<DIM>> clone() const
        {
            return std::make_shared<HistogramFunction<DIM>>(*_histFct);
        }
        /*
        HistogramFunction<DIM> operator+(const HistogramFunctionInterface<DIM>& histFct) const
        {
            return AddHist<DIM>(_histFct, std::shared_ptr<HistogramFunctionInterface<DIM>>(histFct.clone()));
        }
        
        HistogramFunction<DIM> operator+=(const HistogramFunctionInterface<DIM>& histFct) const
        {
            return AddHist<DIM>(_histFct, std::shared_ptr<HistogramFunctionInterface<DIM>>(histFct.clone()));
        }
        */
        
        HistogramFunction<DIM> operator+(const HistogramInterface<DIM>& histFct) const;
};

template<sizetype DIM>
class AddHist:
    public HistogramInterface<DIM>
{
    protected:
        const std::shared_ptr<HistogramInterface<DIM>> _lhs;
        const std::shared_ptr<HistogramInterface<DIM>> _rhs;
        
    public:
        AddHist(const HistogramInterface<DIM>& lhs, const HistogramInterface<DIM>& rhs):
            _lhs(lhs.clone()),
            _rhs(rhs.clone())
        {
            //std::cout<<"LHS: "<<_lhs.get()<<", RHS: "<<_rhs.get()<<std::endl;
            
            //TODO: check binning already here & not during getHistogram()
        }
        
        virtual Histogram<DIM> get() const
        {
            std::cout<<"LHS: "<<_lhs.get()<<", RHS: "<<_rhs.get()<<std::endl;
            Histogram<DIM> hist = _lhs->get();
            hist.add(_rhs->get());
            return hist;
        }
        
        virtual std::shared_ptr<HistogramInterface<DIM>> clone() const
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
