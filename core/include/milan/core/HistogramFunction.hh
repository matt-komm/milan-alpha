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
class HistogramFunctionInterface
{
    protected:
    public:
        virtual Histogram<DIM> getHistogram() const = 0;
        virtual std::shared_ptr<HistogramFunctionInterface<DIM>> clone() const = 0;
};

template<sizetype DIM>
class HistogramFunction:
    public HistogramFunctionInterface<DIM>
{
    protected:
        const std::shared_ptr<HistogramFunctionInterface<DIM>> _histFct;
    public:
        HistogramFunction<DIM>(HistogramFunctionInterface<DIM>&& histFct):
            _histFct(histFct.clone())
        {
            //std::cout<<"by copy"<<std::endl;
        }
        
        HistogramFunction<DIM>(const HistogramFunctionInterface<DIM>& histFct):
            _histFct(histFct.clone())
        {
            //TODO: many make no copy here
            //std::cout<<"by ref"<<std::endl;
        }
    
        virtual Histogram<DIM> getHistogram() const
        {
            return _histFct->getHistogram();
        }
        
        virtual std::shared_ptr<HistogramFunctionInterface<DIM>> clone() const
        {
            return std::make_shared<HistogramFunction<DIM>>(*_histFct);
        }
        

};


template<sizetype DIM>
class AddHist:
    public HistogramFunctionInterface<DIM>
{
    protected:
        HistogramFunctionInterface<DIM>* _lhs;
        HistogramFunctionInterface<DIM>* _rhs;
        
    public:
        AddHist(HistogramFunctionInterface<DIM>* lhs, HistogramFunctionInterface<DIM>* rhs):
            _lhs(lhs),
            _rhs(rhs)
        {
        }
        
        virtual Histogram<DIM> getHistogram() const
        {
            Histogram<DIM> hist = _lhs->getHistogram();
            //hist+=_rhs->getHistogram();
            return hist;
        }
        
        virtual std::shared_ptr<HistogramFunctionInterface<DIM>> clone() const
        {
            return std::make_shared<AddHist<DIM>>(_lhs,_rhs);
        }
};

template<sizetype DIM>
HistogramFunction<DIM> operator+(HistogramFunctionInterface<DIM>& lhs, HistogramFunctionInterface<DIM>& rhs)
{
    return AddHist<DIM>(&lhs,&rhs);
}

}

#endif
