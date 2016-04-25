#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"

#include "Eigen/Eigen"

#include <vector>

namespace milan
{

class Histogram
{
    protected:
        Eigen::VectorXf _content;
        Eigen::VectorXf _uncertainty2;
    public:
        Histogram(const sizetype& N):
            _content(Eigen::VectorXf::Zero(N)),
            _uncertainty2(Eigen::VectorXf::Zero(N))
        {
        }
        
        Histogram& operator*=(const float32& factor)
        {
            _content*=factor;
            _uncertainty2*=factor*factor;
            return *this;
        }
};


}

#endif

