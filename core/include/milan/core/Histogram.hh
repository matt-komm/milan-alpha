#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"
#include "milan/core/Binning.hh"
#include "milan/core/Exception.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/HistogramFunction.hh"

#include <array>
#include <cmath>
#include <vector>

namespace milan
{

class Histogram:
    public HistogramInterface,
    public PtrInterface<HistogramInterface,Histogram>
{
    protected:
        std::vector<Binning> _binning;
        
        std::vector<double> _content;
        std::vector<double> _error2;
    public:
        Histogram(const std::vector<Binning>& binning):
            _binning(binning)
        {   
            sizetype N = 1;
            for (sizetype idim = 0; idim < _binning.size(); ++idim)
            {
                //add 2 to account for over- & underflow
                N*=_binning[idim].size()+2;
            }
            _content = std::vector<double>(N,0);
            _error2 = std::vector<double>(N,0);
        }
        
        Histogram(const Histogram& histogram):
            _binning(histogram._binning),
            _content(histogram._content),
            _error2(histogram._error2)
        {
        }
        
        Histogram& operator=(const Histogram& histogram)
        {
            _binning = histogram._binning;
            _content = histogram._content;
            _error2 = histogram._error2;
            return *this;
        }
        
        Histogram(Histogram&& histogram):
            _binning(std::move(histogram._binning)),
            _content(std::move(histogram._content)),
            _error2(std::move(histogram._error2))
        {
        }
        
        Histogram& operator=(Histogram&& histogram)
        {
            _binning = std::move(histogram._binning);
            _content = std::move(histogram._content);
            _error2 = std::move(histogram._error2);
            return *this;
        }
        
        virtual sizetype size() const
        {
            return _content.size();
        }
        
        inline double getContent(const std::vector<sizetype>& index) const
        {
            return _content[getGlobalBinFromIndex(index)];
        }
        
        inline void setContent(const std::vector<sizetype>& index, const double& value)
        {
            _content[getGlobalBinFromIndex(index)] = value;
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
        
        inline void setError2(sizetype index, double error2)
        {
            if (error2<0.0)
            {
                milan_throw("Error2 of a histogram needs to be positive - not ",error2,"!");
            }
            _error2[index]=error2;
        }
        
        inline const Binning& getBinning(const sizetype& idim) const
        {
            return _binning[idim];
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _binning;
        }
        
        sizetype getGlobalBinFromIndex(const std::vector<sizetype>& index) const
        {
            sizetype globalIndex = 0;
            sizetype offset = 1;
            for (sizetype idim = 0; idim < _binning.size(); ++idim)
            {
                globalIndex+=index[idim]*offset;
                offset*=_binning[idim].size()+2;
            }
            return globalIndex;
        }
        
        inline std::vector<sizetype> findIndexFromValue(const std::vector<double>& value) const
        {
            std::vector<sizetype> index(_binning.size(),0);
            for (sizetype idim = 0; idim < _binning.size(); ++idim)
            {
                index[idim]=_binning[idim].findBin(value[idim]);
            }
            return index;
        }
        
        inline sizetype findGlobalBinFromValue(const std::vector<double>& value) const
        {
            return getGlobalBinFromIndex(findIndexFromValue(value));
        }
        
        virtual double getContent(sizetype index) const
        {
            return _content[index];
        }
        
        virtual double getDerivative(sizetype, const Ptr<Parameter>&) const
        {
            return 0.0;
        }
        
        virtual double getError2(sizetype index) const
        {
            return std::max(0.0,_error2[index]);
        }
        
        Histogram operator+(const Histogram& rhs) const
        {
            Histogram result(*this); //copy
            for (sizetype ibin = 0; ibin < result._content.size(); ++ibin)
            {
                result._content[ibin]+=rhs._content[ibin];
                result._error2[ibin]+=rhs._error2[ibin];
            }
            return result;
        }
        
        Histogram operator-(const Histogram& rhs) const
        {
            Histogram result(*this); //copy
            for (sizetype ibin = 0; ibin < result._content.size(); ++ibin)
            {
                result._content[ibin]-=rhs._content[ibin];
                result._error2[ibin]+=rhs._error2[ibin];
            }
            return result;
        }
        
        Histogram operator*(double factor) const
        {
            Histogram result(*this); //copy
            for (sizetype ibin = 0; ibin < result._content.size(); ++ibin)
            {
                result._content[ibin]*=factor;
                result._error2[ibin]*=factor;
            }
            return result;
        }
    
        Histogram operator/(double factor) const
        {
            Histogram result(*this); //copy
            for (sizetype ibin = 0; ibin < result._content.size(); ++ibin)
            {
                result._content[ibin]/=factor;
                result._error2[ibin]/=factor;
            }
            return result;
        }
        
        inline operator HistogramFunction() const
        {
            return copy();
        }
        
        virtual ~Histogram()
        {
        }
};

}

#endif

