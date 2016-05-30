#ifndef __MILAN_CORE_BINNING_H__
#define __MILAN_CORE_BINNING_H__

#include "milan/core/Types.hh"
#include "milan/core/Exception.hh"

#include <vector>
#include <algorithm>
#include <limits>

namespace milan
{

class Binning
{
    protected:
        std::vector<double> _binEdges;
    public:
        Binning(sizetype N, double start, double end):
            _binEdges(N+1)
        {
            if (N==0)
            {
                milan_throw("Binning scheme with 0 bins is meaningless");
            }
            if (end<=start)
            {
                milan_throw("Attempt to define binning with end<=start: ",end,"<=",start);
            }
            for (sizetype ibin = 0; ibin < N+1; ++ibin)
            {
                _binEdges[ibin]=start+1.0*ibin/N*(end-start);
            }
        }
        
        Binning(const std::initializer_list<double> binEdges):
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
                //allow the difference to be equal to epsilon at the most
                if (std::fabs(_binEdges[ibin]-binning._binEdges[ibin])>std::numeric_limits<double>::epsilon())
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
        
        inline double getBinCenter(sizetype index) const
        {
            return 0.5*(_binEdges[index-1]+_binEdges[index]);
        }
        
        inline double getBinLowerEdge(sizetype index) const
        {
            return _binEdges[index-1];
        }
        
        inline double getBinUpperEdge(sizetype index) const
        {
            return _binEdges[index];
        }
        
        inline double getBinWidth(sizetype index) const
        {
            return _binEdges[index]-_binEdges[index-1];
        }
        
        sizetype findBin(double value) const
        {
        
            if (_binEdges.front()>value)
            {
                return 0;
            }
            else if (_binEdges.back()<value)
            {
                return size()+1;
            }
            
            //TODO: binary search
            /*
            sizetype first = 1;
            sizetype last = size()+2;
            sizetype middle = (first+last)/2;

            while (first <= last) 
            {
                if (getBinUpperEdge(middle) < value)
                {
                    first = middle + 1;
                }
                else if (getBinLowerEdge(middle)<=value and getBinUpperEdge(middle)>value)
                {
                    return middle;
                }   
                else
                {
                    last = middle - 1;
                }
                middle = (first + last)/2;
            }
            */
            
            for (sizetype ibin = 1; ibin < _binEdges.size(); ++ibin)
            {
                if (getBinLowerEdge(ibin)<=value and getBinUpperEdge(ibin)>value)
                {
                    return ibin;
                }
            }
            
            return size()+1;
        }
};

}

#endif

