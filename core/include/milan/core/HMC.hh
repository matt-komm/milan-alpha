#ifndef __MILAN_CORE_HMC_H__
#define __MILAN_CORE_HMC_H__

#include "milan/core/Types.hh"

#include <functional>

namespace milan
{

class HMC
{
    protected:
        std::vector<double*> _parameters;
    public:
        HMC()
        {
        }
        
        void addParameter(double* x)
        {
            _parameters.push_back(x);
        }
        
        void step(std::function<double()> fct, std::function<double(sizetype i)> fctGrad, double eps, sizetype L)
        {
            
        }
           
};

}

#endif
