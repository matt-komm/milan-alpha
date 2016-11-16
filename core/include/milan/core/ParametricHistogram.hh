#ifndef __MILAN_CORE_PARAMETRICHISTOGRAM_H__
#define __MILAN_CORE_PARAMETRICHISTOGRAM_H__

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

class ParametricHistogram:
    public HistogramInterface,
    public PtrInterface<HistogramInterface,ParametricHistogram>
{
    protected:
        std::vector<Binning> _binning;
        std::vector<Ptr<Parameter>> _content;
        
    public:
        ParametricHistogram(const std::vector<Binning>& binning, std::string parameterNamePrefix, double value=1, double min=Parameter::MIN, double max=Parameter::MAX, double step = 0.1):
            _binning(binning)
        {   
            sizetype N = 1;
            for (sizetype idim = 0; idim < _binning.size(); ++idim)
            {
                //add 2 to account for over- & underflow
                N*=_binning[idim].size()+2;
            }
            _content.reserve(N);
            for (sizetype i = 0; i < N; ++i)
            {
                _content.push_back(Ptr<Parameter>(PtrStorage::OWN,new Parameter(parameterNamePrefix+"@bin"+std::to_string(i),value,min,max,step)));
            }
        }
        
        ParametricHistogram(const std::vector<Binning>& binning, const std::vector<Ptr<Parameter>>& parameters):
            _binning(binning),
            _content(parameters)
        {   
            sizetype N = 1;
            for (sizetype idim = 0; idim < _binning.size(); ++idim)
            {
                //add 2 to account for over- & underflow
                N*=_binning[idim].size()+2;
            }
            
            if (N!=parameters.size())
            {
                milan_throw("Number of parameters (",parameters.size(),") does not match binning scheme including under- and overflow bins (",N,")");
            }
        }
        
        ParametricHistogram(const ParametricHistogram& histogram):
            _binning(histogram._binning),
            _content(histogram._content)
        {
        }
        
        ParametricHistogram& operator=(const ParametricHistogram& histogram)
        {
            _binning = histogram._binning;
            _content = histogram._content;
            return *this;
        }
        
        ParametricHistogram(ParametricHistogram&& histogram):
            _binning(std::move(histogram._binning)),
            _content(std::move(histogram._content))
        {
        }
        
        ParametricHistogram& operator=(ParametricHistogram&& histogram)
        {
            _binning = std::move(histogram._binning);
            _content = std::move(histogram._content);
            return *this;
        }
        
        virtual sizetype size() const
        {
            return _content.size();
        }
        
        inline double getContent(const std::vector<sizetype>& index) const
        {
            return _content[getGlobalBinFromIndex(index)]->getValue();
        }
        
        inline const Ptr<Parameter>& getParameter(const std::vector<sizetype>& index) const
        {
            return _content[getGlobalBinFromIndex(index)];
        }
        
        inline const Ptr<Parameter>& getParameter(sizetype index) const
        {
            return _content[index];
        }
        
        inline const std::vector<Ptr<Parameter>>& getParameters() const
        {
            return _content;
        }
        
        inline void setContent(const std::vector<sizetype>& index, const double& value)
        {
            _content[getGlobalBinFromIndex(index)]->setValue(value);
        }
        
        inline void setContent(const sizetype& globalIndex, const double& value)
        {
            _content[globalIndex]->setValue(value);
        }
        
        inline double getError(const std::vector<sizetype>&) const
        {
            return 0.0;
        }
        
        inline double getError2(const std::vector<sizetype>&) const
        {
            return 0.0;
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
            return _content[index]->getValue();
        }
        
        virtual double getDerivative(sizetype index, const Ptr<Parameter>& parameter) const
        {
            return _content[index].get()==parameter.get();
        }
        
        virtual double getError2(sizetype) const
        {
            return 0.0;
        }
        
        inline operator HistogramFunction() const
        {
            return copy();
        }
};

}

#endif

