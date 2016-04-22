#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include <cstddef>
#include <vector>

namespace milan
{

class Histogram
{
    protected:
        const std::size_t _N;
    
        std::vector<float> _content;
    public:
        Histogram(std::size_t N):
            _N(N),
            _content(0,N)
        {
            _content.reserve(N);
        }
        
        Histogram& operator*=(float factor)
        {
            for (unsigned int i = 0; i < _N; ++i)
            {
                _content[i]*=factor;
            }
            return *this;
        }
};

}

#endif

