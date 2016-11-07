#ifndef __MILAN_CORE_HISTOGRAMOPERATORS_H__
#define __MILAN_CORE_HISTOGRAMOPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/FunctionInterface.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class HistogramAddOperator:
    public HistogramInterface
{
    protected:
        const Ptr<const HistogramInterface> _lhs;
        const Ptr<const HistogramInterface> _rhs;
        
    public:
        HistogramAddOperator(
            const Ptr<const HistogramInterface>& lhs, 
            const Ptr<const HistogramInterface>& rhs
        ):
            _lhs(lhs),
            _rhs(rhs)
        {
            if (lhs->getBinningVector()!=rhs->getBinningVector())
            {
                milan_throw("Cannot add Histogram: Found mismatching binnings!");
            }
        }
        
        virtual sizetype size() const
        {
            return _lhs.get()->size();
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _lhs.get()->getBinningVector();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _lhs.get()->getContent(index)+_rhs.get()->getContent(index);
        }
        
        virtual double getDifferential(sizetype index, const Ptr<Parameter>& parameter) const
        {
            return _lhs.get()->getDifferential(index,parameter)+_rhs.get()->getDifferential(index,parameter);
        }
        
        virtual double getError2(sizetype index) const
        {
            return _lhs.get()->getError2(index)+_rhs.get()->getError2(index);
        }
};

class ParameterHistogramMultiplicationOperator:
    public HistogramInterface
{
    protected:
        const Ptr<const HistogramInterface> _histogram;
        const Ptr<const FunctionInterface> _function;
        
    public:
        ParameterHistogramMultiplicationOperator(
            const Ptr<const HistogramInterface>& histogram,
            const Ptr<const FunctionInterface>& function
        ):
            _histogram(histogram),
            _function(function)
        {
        }
        
        virtual sizetype size() const
        {
            return _histogram->size();
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _histogram->getBinningVector();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _histogram.get()->getContent(index)*_function->getValue();
        }
        
        virtual double getDifferential(sizetype index, const Ptr<Parameter>& parameter) const
        {
            double result = 0;
            const double histContent = _histogram->getContent(index);
            const double functionValue = _function->getValue();
            if (histContent>0) result+=_function->getDifferential(parameter)*histContent;
            if (functionValue>0) result+=functionValue*_histogram->getDifferential(index,parameter);
            return result;
        }
        
        virtual double getError2(sizetype index) const
        {
            return _histogram->getError2(index)*_function->getValue()*_function->getValue();
        }
};

}


#endif
