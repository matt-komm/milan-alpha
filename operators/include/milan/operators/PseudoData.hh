#ifndef __MILAN_OPERATORS_PSEUDODATA_H__
#define __MILAN_OPERATORS_PSEUDODATA_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Histogram.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class PseudoData:
    public HistogramInterface,
    public PtrInterface<HistogramInterface,PseudoData>
{
    protected:
        const Ptr<const HistogramInterface> _source;
        Histogram _generatedHistogram;
        
    public:
        PseudoData(const Ptr<const HistogramInterface>& source):
            _source(source),
            _generatedHistogram(source.get()->getBinningVector())
        {
            generate();
        }
        
        void generate()
        {
            for (unsigned int ibin = 0; ibin < _generatedHistogram.size(); ++ibin)
            {
                _generatedHistogram.setContent(ibin,_source.get()->getContentValue(ibin));
                _generatedHistogram.setError2(ibin,_source.get()->getError2(ibin));
            }
        }
        
        virtual Histogram getResult() const
        {
            return _generatedHistogram;
        }
        
        virtual sizetype size() const
        {
            return _generatedHistogram.size();
        }
        
        virtual double getContentValue(sizetype index) const
        {
            return _generatedHistogram.getContentValue(index);
        }
        
        virtual Ftype getContentFtype(sizetype index) const
        {
            return _generatedHistogram.getContentValue(index);
        }
        
        virtual double getError2(sizetype index) const
        {
            return _generatedHistogram.getError2(index);
        }
        
        virtual const std::vector<Binning>& getBinningVector() const
        {
            return _generatedHistogram.getBinningVector();
        }
        
        
};

}

#endif
