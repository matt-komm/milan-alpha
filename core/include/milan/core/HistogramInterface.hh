#ifndef __MILAN_CORE_HISTOGRAMINTERFACE_H__
#define __MILAN_CORE_HISTOGRAMINTERFACE_H__

#include "milan/core/Types.hh"
#include "milan/core/Binning.hh"
#include "milan/core/Parameter.hh"

#include <iostream>
#include <memory>

namespace milan
{

class HistogramInterface
{
    protected:
    public:
        //virtual Histogram getResult() const = 0;
        virtual sizetype size() const = 0; //this refers to the resulting type in case of operations!
        virtual double getContent(sizetype index) const = 0;
        virtual double getDifferential(sizetype index, const Parameter& p) const = 0;
        virtual double getError2(sizetype index) const = 0;
        
        //TODO: make binning const
        virtual const std::vector<Binning>& getBinningVector() const = 0; //this refers to the resulting type in case of operations!
        
        virtual sizetype getDimension() const
        {
            return this->getBinningVector().size();
        }
        
        
        
        virtual ~HistogramInterface()
        {
        }
};

}

#endif
