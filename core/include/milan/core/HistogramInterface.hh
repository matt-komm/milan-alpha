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
        
        virtual sizetype size() const = 0; //this refers to the resulting type in case of operations!
        virtual double getContent(sizetype index) const = 0;
        
        //For v2:
        //virtual void getContent(sizetype index, sizetype length, double_aligned* __restrict__ content)
        //{
        //}
        
        virtual double getDerivative(sizetype index, const Ptr<Parameter>& parameter) const = 0;
        //TODO: implement also 
        //virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        //however speedup may not be large
        
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
