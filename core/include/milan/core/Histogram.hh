#ifndef __MILAN_CORE_HISTOGRAM_H__
#define __MILAN_CORE_HISTOGRAM_H__

#include "milan/core/Types.hh"

#include "Eigen/Eigen"

namespace milan
{
/*
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
*/

class Histogram
{
    protected:
        std::vector<float> _content;
        std::vector<float> _uncertainty2;
    public:
        Histogram(const sizetype& N):
            _content(N,0),
            _uncertainty2(N,0)
        {
        }
        
        Histogram& operator*=(const float32& factor)
        {
            for (unsigned int i = 0; i < _content.size(); ++i)
            {
                _content[i]*=factor;
                _uncertainty2[i]*=factor*factor;
            }
            return *this;
        }
};

}

#endif

