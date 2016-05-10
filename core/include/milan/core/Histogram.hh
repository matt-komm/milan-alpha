#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"
#include "milan/core/Binning.hh"
#include "milan/core/Exception.hh"

#include <array>
#include <cmath>
#include <vector>
#include <initializer_list>
#include <iostream>

namespace milan
{

template<class TYPE, sizetype DIM>
class Histogram
{
    protected:
        const std::vector<Binning<TYPE>> _binning;
        
        std::vector<TYPE> _content;
        std::vector<TYPE> _error2;
    public:
        Histogram(const std::vector<Binning<TYPE>>& binning):
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
            _content = std::vector<TYPE>(N,0);
            _error2 = std::vector<TYPE>(N,0);
        }
        
        inline TYPE getContent(const std::vector<sizetype>& index) const
        {
            return _content[getGlobalBinFromIndex(index)];
        }
        
        inline void setContent(const std::vector<sizetype>& index, const TYPE& value)
        {
            _content[getGlobalBinFromIndex(index)] = value;
        }
        
        inline TYPE getContent(const sizetype& globalIndex) const
        {
            return _content[globalIndex];
        }
        
        inline void setContent(const sizetype& globalIndex, const TYPE& value)
        {
            _content[globalIndex] = value;
        }
        
        inline TYPE getError(const std::vector<sizetype>& index) const
        {
            return std::sqrt(getError2(index));
        }
        
        inline TYPE getError2(const std::vector<sizetype>& index) const
        {
            return _error2[getGlobalBinFromIndex(index)];
        }
        
        inline void setError(const std::vector<sizetype>& index, const TYPE& error)
        {
            setError2(index,error*error);
        }
        
        inline void setError2(const std::vector<sizetype>& index, const TYPE& error2)
        {
            if (error2<0.0)
            {
                milan_throw("Error2 of a histogram needs to be positive - not ",error2,"!");
            }
            _error2[getGlobalBinFromIndex(index)] = error2;
        }
        
        inline const Binning<TYPE>& getBinning(const sizetype& idim) const
        {
            return _binning[idim];
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
        
        inline std::vector<sizetype> findIndexFromValue(const std::vector<TYPE>& value) const
        {
            if (value.size()!=DIM)
            {
                milan_throw("Value dimension ",value.size()," cannot be used to fill histogram with dimension ",DIM);
            }
            std::vector<sizetype> index(DIM,0);
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                index[idim]=_binning.findBin(value[idim]);
            }
            return std::move(index);
        }
        
        inline sizetype findGlobalBinFromValue(const std::vector<TYPE>& value) const
        {
            return getGlobalBinFromIndex(findIndexFromValue(value));
        }
};

}

#endif

