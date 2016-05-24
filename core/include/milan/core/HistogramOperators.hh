#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

template<sizetype DIM>
class HistogramAddOperator:
    public HistogramInterface<DIM>
{
    protected:
        const Ptr<const HistogramInterface<DIM>> _lhs;
        const Ptr<const HistogramInterface<DIM>> _rhs;
        
    public:
        HistogramAddOperator(const Ptr<const HistogramInterface<DIM>>& lhs, const Ptr<const HistogramInterface<DIM>>& rhs):
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
        
        virtual Histogram<DIM> getResult() const
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

template<sizetype DIM>
class ParameterHistogramMultiplicationOperator:
    public HistogramInterface<DIM>
{
    protected:
        const Ptr<const HistogramInterface<DIM>> _histogram;
        const Ptr<const Parameter> _parameter;
        
        
    public:
        ParameterHistogramMultiplicationOperator(const Ptr<const HistogramInterface<DIM>>& histogram,const Ptr<const Parameter>& parameter):
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
        
        virtual Histogram<DIM> getResult() const
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
