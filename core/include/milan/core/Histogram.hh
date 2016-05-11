#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"
#include "milan/core/Binning.hh"
#include "milan/core/Exception.hh"
#include "milan/core/HistogramFunction.hh"

#include <array>
#include <cmath>
#include <vector>

namespace milan
{

template<sizetype DIM>
class Histogram:
    public HistogramFunctionInterface<DIM>
{
    protected:
        const std::vector<Binning> _binning;
        
        std::vector<double> _content;
        std::vector<double> _error2;
    public:
        Histogram(const std::vector<Binning>& binning):
            _binning(binning)
        {   
            if (binning.size()!=DIM)
            {
                milan_throw("Histogram is of dimension ",DIM," but a binning scheme with ",binning.size()," was configured");
            }
            sizetype N = 1;
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                //add 2 to account for over- & underflow
                N*=_binning[idim].size()+2;
            }
            _content = std::vector<double>(N,0);
            _error2 = std::vector<double>(N,0);
        }
        
        inline double getContent(const std::vector<sizetype>& index) const
        {
            return _content[getGlobalBinFromIndex(index)];
        }
        
        inline void setContent(const std::vector<sizetype>& index, const double& value)
        {
            _content[getGlobalBinFromIndex(index)] = value;
        }
        
        inline double getContent(const sizetype& globalIndex) const
        {
            return _content[globalIndex];
        }
        
        inline void setContent(const sizetype& globalIndex, const double& value)
        {
            _content[globalIndex] = value;
        }
        
        inline double getError(const std::vector<sizetype>& index) const
        {
            return std::sqrt(getError2(index));
        }
        
        inline double getError2(const std::vector<sizetype>& index) const
        {
            return _error2[getGlobalBinFromIndex(index)];
        }
        
        inline void setError(const std::vector<sizetype>& index, const double& error)
        {
            setError2(index,error*error);
        }
        
        inline void setError2(const std::vector<sizetype>& index, const double& error2)
        {
            if (error2<0.0)
            {
                milan_throw("Error2 of a histogram needs to be positive - not ",error2,"!");
            }
            _error2[getGlobalBinFromIndex(index)] = error2;
        }
        
        inline const Binning& getBinning(const sizetype& idim) const
        {
            return _binning[idim];
        }
        
        inline const std::vector<Binning>& getBinningVector() const
        {
            return _binning;
        }
        
        sizetype getGlobalBinFromIndex(const std::vector<sizetype>& index) const
        {
            sizetype globalIndex = 0;
            sizetype offset = 1;
            if (index.size()!=DIM)
            {
                milan_throw("Index dimension ",index.size()," cannot be used to access histogram with dimension ",DIM);
            }
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                globalIndex+=index[idim]*offset;
                offset*=_binning[idim].size()+2;
            }
            return globalIndex;
        }
        
        inline std::vector<sizetype> findIndexFromValue(const std::vector<double>& value) const
        {
            if (value.size()!=DIM)
            {
                milan_throw("Value dimension ",value.size()," cannot be used to fill histogram with dimension ",DIM);
            }
            std::vector<sizetype> index(DIM,0);
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                index[idim]=_binning[idim].findBin(value[idim]);
            }
            return std::move(index);
        }
        
        inline sizetype findGlobalBinFromValue(const std::vector<double>& value) const
        {
            return getGlobalBinFromIndex(findIndexFromValue(value));
        }
        
        Histogram<DIM>& add(const Histogram<DIM>& histogram, bool checkBinning=true)
        {
            return *this;
        }
        
        Histogram<DIM>& multiply(const double& factor)
        {
            return *this;
        }
        
        virtual Histogram<DIM> getHistogram() const
        {
            return *this;
        }
        
        virtual std::shared_ptr<HistogramFunctionInterface<DIM>> clone() const
        {
            return std::make_shared<Histogram<DIM>>(_binning);
        }
};

}

#endif

