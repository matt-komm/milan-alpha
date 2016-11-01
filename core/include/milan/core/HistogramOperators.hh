#ifndef __MILAN_CORE_HISTOGRAMOPERATORS_H__
#define __MILAN_CORE_HISTOGRAMOPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
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
        HistogramAddOperator(const Ptr<const HistogramInterface>& lhs, const Ptr<const HistogramInterface>& rhs):
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
        
        virtual double getContentValue(sizetype index) const
        {
            return _lhs.get()->getContentValue(index)+_rhs.get()->getContentValue(index);
        }
        
        virtual Ftype getContentFtype(sizetype index) const
        {
            return _lhs.get()->getContentFtype(index)+_rhs.get()->getContentFtype(index);
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
        const Ptr<const Parameter> _parameter;
        
        
    public:
        ParameterHistogramMultiplicationOperator(const Ptr<const HistogramInterface>& histogram,const Ptr<const Parameter>& parameter):
            _histogram(histogram),
            _parameter(parameter)
        {
        }
        
        virtual sizetype size() const
        {
            return _histogram.get()->size();
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _histogram->getBinningVector();
        }
        
        virtual double getContentValue(sizetype index) const
        {
            return _histogram.get()->getContentValue(index)*_parameter.get()->getValue();
        }
        
        virtual Ftype getContentFtype(sizetype index) const
        {
            return _histogram.get()->getContentFtype(index)*_parameter.get()->getFtype();
        }
        
        virtual double getError2(sizetype index) const
        {
            return _histogram.get()->getError2(index)*_parameter.get()->getValue()*_parameter.get()->getValue();
        }
};

}


#endif
