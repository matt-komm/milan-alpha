#ifndef __MILAN_CORE_BINNING_H__
#define __MILAN_CORE_BINNING_H__

#include "milan/core/Types.hh"
#include "milan/core/Exception.hh"

#include <vector>
#include <algorithm>

namespace milan
{

class Binning
{
    protected:
        std::vector<float64> _binEdges;
    public:
        Binning(const sizetype& N, const float64& start, const float64& end):
            _binEdges(N+1)
        {
            if (end<=start)
            {
                milan_throw("Attempt to define binning with end<=start: ",end,"<=",start);
            }
            for (sizetype ibin = 0; ibin < N+1; ++ibin)
            {
                _binEdges[ibin]=start+1.0*ibin/N*(end-start);
            }
        }
        
        Binning(const std::initializer_list<float64> binEdges):
            _binEdges(binEdges)
        {
            for (sizetype ibin = 0; ibin < _binEdges.size()-1; ++ibin)
            {
                if (_binEdges[ibin]>=_binEdges[ibin+1])
                {
                    milan_throw("Binning edges need to be monotonous but found value pair ",_binEdges[ibin],"<=",_binEdges[ibin+1], "at index ",ibin+1);
                }
            }
        }
        
        bool operator==(const Binning& binning) const
        {
            if (_binEdges.size()!=binning._binEdges.size())
            {
                return false;
            }
            for (sizetype ibin = 0; ibin < _binEdges.size()-1; ++ibin)
            {
                if (_binEdges[ibin]!=binning._binEdges[ibin])
                {
                    return false;
                }
            }
            return true;
        }
        
        inline bool operator!=(const Binning& binning) const
        {
            return not ((*this)==binning);
        }
        
        inline sizetype size() const
        {
            return _binEdges.size()-1;
        }
        
        inline float64 getBinCenter(sizetype index) const
        {
            return 0.5*(_binEdges[index]+_binEdges[index+1]);
        }
        
        inline float64 getBinLowerEdge(sizetype index) const
        {
            return _binEdges[index];
        }
        
        inline float64 getBinUpperEdge(sizetype index) const
        {
            return _binEdges[index+1];
        }
        
        inline float64 getBinWidth(sizetype index) const
        {
            return _binEdges[index+1]-_binEdges[index];
        }
        
        sizetype findBin(const float64& value) const
        {
            /*
            TODO: binary search
            first = 0;
            last = n - 1;
            middle = (first+last)/2;

            while (first <= last) {
              if (array[middle] < search)
                 first = middle + 1;    
              else if (array[middle] == search) {
                 printf("%d found at location %d.\n", search, middle+1);
                 break;
              }
              else
                 last = middle - 1;

              middle = (first + last)/2;
            }
               
            */
            if (_binEdges.front()>value)
            {
                return 0;
            }
            for (sizetype ibin = 0; ibin < _binEdges.size()-1; ++ibin)
            {
                if (_binEdges[ibin]<=value and _binEdges[ibin]>value)
                {
                    return ibin+1;
                }
            }
            return _binEdges.size()+1;
        }
};

}

#endif

