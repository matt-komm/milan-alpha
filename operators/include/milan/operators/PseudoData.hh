#ifndef __MILAN_OPERATORS_PSEUDODATA_H__
#define __MILAN_OPERATORS_PSEUDODATA_H__

#include "milan/core/Types.hh"
#include "milan/core/HistogramInterface.hh"
#include "milan/core/Histogram.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

template<sizetype DIM>
class PseudoData:
    public HistogramInterface<DIM>
{
    protected:
        const Ptr<const HistogramInterface<DIM>> _source;
        Histogram<DIM> _generatedHistogram;
        
    public:
        PseudoData(const Ptr<const HistogramInterface<DIM>>& source):
            _generatedHistogram(source->getBinningVector())
        {
            generate();
        }
        
        void generate()
        {
            for (unsigned int ibin = 0; ibin < _generatedHistogram.size(); ++ibin)
            {
                _generatedHistogram.setContent(ibin,_source.get()->getContent(ibin));
                _generatedHistogram.setError2(ibin,_source.get()->getError2(ibin));
            }
        }
        
        virtual Histogram<DIM> getResult() const
        {
            return _generatedHistogram;
        }
        
        virtual sizetype size() const
        {
            return _generatedHistogram.size();
        }
        
        virtual double getContent(sizetype index) const
        {
            return _generatedHistogram.getContent(index);
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
