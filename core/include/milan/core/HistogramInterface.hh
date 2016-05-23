#ifndef __MILAN_CORE_HISTOGRAMINTERFACE_H__
#define __MILAN_CORE_HISTOGRAMINTERFACE_H__

#include "milan/core/Types.hh"

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
        virtual Histogram<DIM> getResult() const = 0;
        virtual sizetype size() const = 0;
        virtual double getContent(sizetype index) const = 0;
        virtual double getError2(sizetype index) const = 0;
        virtual const std::vector<Binning>& getBinningVector() const = 0;
        
        virtual ~HistogramInterface()
        {
        }
};

}

#endif
