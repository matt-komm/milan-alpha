#ifndef __MILAN_CORE_OPERATORS_H__
#define __MILAN_CORE_OPERATORS_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/HistogramFunction.hh"
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
            
        }
        
        virtual sizetype size() const
        {
            return _lhs.get()->size();
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

}


#endif
