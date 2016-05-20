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
        virtual Histogram<DIM> get() const = 0;
        
        virtual ~HistogramInterface()
        {
        }
};

}

#endif
