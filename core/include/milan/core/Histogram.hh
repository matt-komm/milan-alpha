#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"
#include "milan/core/Binning.hh"
#include "milan/core/Exception.hh"

#include <array>
#include <cmath>

namespace milan
{

template<class TYPE, sizetype DIM>
class Histogram
{
    public:
        typedef Binning<TYPE> B;
    protected:
        std::array<Binning<TYPE>,DIM> _binning;
        
        std::vector<TYPE> _content;
        std::vector<TYPE> _error2;
    public:
        template<class... ARGS>
        Histogram(ARGS&&... args):
            _binning{{std::forward<ARGS>(args)...}}
        {
            static_assert(
                DIM==sizeof...(args),
                "Histogram needs to be initialized with same number of binning objects as there are dimensions"
            );
            sizetype N = 1;
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                N*=_binning[idim].size()+2;
            }
            _content = std::vector<TYPE>(N,0);
            _error2 = std::vector<TYPE>(N,0);
        }
        
        inline TYPE getContent(const std::array<sizetype,DIM>& index) const
        {
            return _content[getGlobalBinFromIndex(index)];
        }
        
        inline void setContent(const std::array<sizetype,DIM>& index, const TYPE& value)
        {
            _content[getGlobalBinFromIndex(index)] = value;
        }
        
        inline TYPE getError(const std::array<sizetype,DIM>& index) const
        {
            return std::sqrt(getError2(index));
        }
        
        inline TYPE getError2(const std::array<sizetype,DIM>& index) const
        {
            return _error2[getGlobalBinFromIndex(index)];
        }
        
        inline void setError(const std::array<sizetype,DIM>& index, const TYPE& error)
        {
            setError2(index,error*error);
        }
        
        inline void setError2(const std::array<sizetype,DIM>& index, const TYPE& error2)
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
        
        sizetype getGlobalBinFromIndex(const std::array<sizetype,DIM>& index) const
        {
            sizetype globalIndex = 0;
            sizetype offset = 1;
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                globalIndex+=index[idim]*offset;
                offset*=_binning[idim].size()+2;
            }
            return globalIndex;
        }
        
        inline std::array<sizetype,DIM> findIndexFromValue(const std::array<TYPE,DIM>& value) const
        {
            std::array<sizetype,DIM> index;
            for (sizetype idim = 0; idim < DIM; ++idim)
            {
                index[idim]=_binning.findBin(value[idim]);
            }
            return index;
        }
        
        inline std::array<sizetype,DIM> findGlobalBinFromValue(const std::array<TYPE,DIM>& value) const
        {
            return getGlobalBinFromIndex(findIndexFromValue(value));
        }
};

template<class TYPE>
class Histogram1:
    public Histogram<TYPE,1>
{
    public:
        Histogram1(const Binning<TYPE>& binning):
            Histogram<TYPE,1>({binning})
        {
        }
        
        inline TYPE getContent(const sizetype& indexX, const sizetype& indexY)
        {
            return this->getContent({indexX,indexY});
        }
};

template<class TYPE>
class Histogram2:
    public Histogram<TYPE,2>
{
    public:
        Histogram2(const Binning<TYPE>& binningX,const Binning<TYPE>& binningY):
            Histogram<TYPE,2>({binningX,binningY})
        {
        }
};

}

#endif

