#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"


#include <iostream>
#include <memory>

namespace milan
{

template<sizetype DIM>
class HistogramFunction:
    public HistogramInterface<DIM>
{
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
    
        HistogramFunction<DIM>(HistogramInterface<DIM>&& hist):
            _histFct(hist.clone())
        {
        }
        
        HistogramFunction<DIM>(const HistogramInterface<DIM>& hist):
            _histFct(hist.clone())
        {
        }
        
        //TODO: prevent deletion
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
        
        virtual std::shared_ptr<const HistogramInterface<DIM>> clone() const
        {
            return std::make_shared<const HistogramFunction<DIM>>(*_histFct);
        }
        
        HistogramFunction<DIM> operator+(const HistogramInterface<DIM>& histFct) const;
};

}

#include "milan/core/Operators.hh"


#endif
