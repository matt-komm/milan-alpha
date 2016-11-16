#ifndef __MILAN_CORE_HISTOGRAMFUNCTION_H__
#define __MILAN_CORE_HISTOGRAMFUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramOperators.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Function.hh"

#include <iostream>
#include <memory>

namespace milan
{

class HistogramFunction:
    public HistogramInterface
{
    protected:
        Ptr<const HistogramInterface> _histFct;
    public:
        HistogramFunction(const HistogramFunction& hist):
            _histFct(hist._histFct)
        {
        }
        
        HistogramFunction(HistogramFunction&& hist):
            _histFct(std::move(hist._histFct))
        {
        }
        
        HistogramFunction& operator=(const HistogramFunction& hist)
        {
            _histFct = hist._histFct;
            return *this;
        }
        
        HistogramFunction& operator=(HistogramFunction&& hist)
        {
            _histFct = std::move(hist._histFct);
            return *this;
        }
        
        HistogramFunction(const Ptr<const HistogramInterface>& hist):
            _histFct(hist)
        {
        }
        
        HistogramFunction& operator=(const Ptr<const HistogramInterface>& hist)
        {
            _histFct = hist;
            return *this;
        }
        
        const inline Ptr<const HistogramInterface>& getPtr() const
        {
            return _histFct;
        }
        
        
        
        virtual sizetype size() const
        {
            return _histFct.get()->size();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _histFct.get()->getContent(index);
        }
        
        virtual double getDerivative(sizetype index, const Ptr<Parameter>& parameter) const
        {
            return _histFct.get()->getDerivative(index,parameter);
        }
        
        virtual double getError2(sizetype index) const
        {
            return _histFct.get()->getError2(index);
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _histFct.get()->getBinningVector();
        }
        
        
        
        HistogramFunction operator+(const HistogramFunction& rhs) const;
        HistogramFunction operator*(const Function& rhs) const;
};

HistogramFunction HistogramFunction::operator+(const HistogramFunction& rhs) const
{
    HistogramAddOperator* op = new HistogramAddOperator(
        this->_histFct,
        rhs._histFct
    );
    Ptr<const HistogramInterface> res(PtrStorage::OWN,op);
    return res;
}

HistogramFunction HistogramFunction::operator*(const Function& rhs) const
{
    ParameterHistogramMultiplicationOperator* op = new ParameterHistogramMultiplicationOperator(
        this->getPtr(),
        rhs.getPtr()
    );
    Ptr<const HistogramInterface> res(PtrStorage::OWN,op);
    return res;
}




}


#endif
