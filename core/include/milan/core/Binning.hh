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
            _binEdges(N)
        {
            if (end<=start)
            {
                milan_throw("Attempt to define binning with end<=start: ",end,"<=",start);
            }
            for (sizetype ibin = 0; ibin < N; ++ibin)
            {
                _binEdges[ibin]=start+1.0*ibin/(N-1)*(end-start);
            }
        }
        
        Binning(const std::vector<float64> binEdges):
            _binEdges(binEdges)
        {
            for (sizetype ibin = 0; ibin < _binEdges.size()-1; ++ibin)
            {
                if (_binEdges[ibin]>=binEdges[ibin+1])
                {
                    milan_throw("Binning edges need to be monotonous but found value pair ",_binEdges[ibin],"<=",_binEdges[ibin+1], "at index ",ibin+1);
                }
            }
        }
        
        inline sizetype size() const
        {
            return _binEdges.size();
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

