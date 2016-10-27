#ifndef __MILAN_CORE_MLFITTER_H__
#define __MILAN_CORE_MLFITTER_H__

#include "milan/core/Types.hh"
#include "milan/core/LikelihoodInterface.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Parameter.hh"

#include <vector>

namespace milan
{

class MLFitter
{
    protected:
        const Ptr<const LikelihoodInterface> _likelihood;
        std::vector<Ptr<Parameter>> _parameters;
    public:
        MLFitter(const Ptr<const LikelihoodInterface>& likelihood):
            _likelihood(likelihood)
        {
        }
        void addParameter(Ptr<Parameter> parameter)
        {
            for (const Ptr<Parameter>& p: _parameters)
            {
                if ((*p)==(*parameter))
                {
                    milan_throw("Attempting to add same parameter '",parameter->getName(),"' twice to fitter");
                }
            }
            _parameters.push_back(parameter);
        }
};

}

#endif
