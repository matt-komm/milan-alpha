#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

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
            //TODO: check binning
        }
        
        virtual sizetype size() const
        {
            return _lhs.get()->size();
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _lhs.get()->getBinningVector();
        }
        
        virtual Histogram getResult() const
        {
            return _lhs.get()->getResult()+_rhs.get()->getResult();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _lhs.get()->getContent(index)+_rhs.get()->getContent(index);
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
            return _histogram.get()->getBinningVector();
        }
        
        virtual Histogram getResult() const
        {
            return _histogram.get()->getResult()*_parameter.get()->getValue();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _histogram.get()->getContent(index)*_parameter.get()->getValue();
        }
        
        virtual double getError2(sizetype index) const
        {
            return _histogram.get()->getError2(index)*_parameter.get()->getValue();
        }
};

}


#endif
