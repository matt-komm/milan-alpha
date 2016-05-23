#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramOperators.hh"
#include "milan/core/Ptr.hh"

#include <iostream>
#include <memory>

namespace milan
{

template<sizetype DIM>
class HistogramFunction:
    public HistogramInterface<DIM>
{
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
        
        virtual sizetype size() const
        {
            return _histFct.get()->size();
        }
        
        virtual Histogram<DIM> getResult() const
        {
            return _histFct.get()->getResult();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _histFct.get()->getContent(index);
        }
        
        virtual double getError2(sizetype index) const
        {
            return _histFct.get()->getError2(index);
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _histFct.get()->getBinningVector();
        }
        
        HistogramFunction<DIM> operator+(const HistogramFunction<DIM>& rhs) const;
        HistogramFunction<DIM> operator*(const Parameter& rhs) const;
};

template<sizetype DIM>
HistogramFunction<DIM> HistogramFunction<DIM>::operator+(const HistogramFunction<DIM>& rhs) const
{
    HistogramAddOperator<DIM>* op = new HistogramAddOperator<DIM>(
        this->_histFct,
        rhs._histFct
    );
    Ptr<const HistogramInterface<DIM>> res(PtrStorage::OWN,op);
    return res;
}

template<sizetype DIM>
HistogramFunction<DIM> HistogramFunction<DIM>::operator*(const Parameter& parameter) const
{
    ParameterHistogramMultiplicationOperator<DIM>* op = new ParameterHistogramMultiplicationOperator<DIM>(
        this->_histFct,
        Ptr<const Parameter>(PtrStorage::SHARE,&parameter)
    );
    Ptr<const HistogramInterface<DIM>> res(PtrStorage::OWN,op);
    return res;
}




}


#endif
