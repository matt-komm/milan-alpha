#ifndef __MILAN_CORE_FUNCTION_H__
#define __MILAN_CORE_FUNCTION_H__

#include "milan/core/Types.hh"
#include "milan/core/Ptr.hh"
#include "milan/core/Parameter.hh"

#include <string>

namespace milan
{

class Function:
    public FunctionInterface
{
    protected:
        Ptr<const FunctionInterface> _fctPtr;
    public:
        Function(const Function& function):
            _fctPtr(function._fctPtr)
        {
        }
        
        Function(Function&& function):
            _fctPtr(std::move(function._fctPtr))
        {
        }
        
        Function& operator=(const Function& function)
        {
            _fctPtr = function._fctPtr;
            return *this;
        }
        
        Function& operator=(Function&& function)
        {
            _fctPtr = std::move(function._fctPtr);
            return *this;
        }
        
        Function(const Ptr<const FunctionInterface>& fctPtr):
            _fctPtr(fctPtr)
        {
        }
        
        Function& operator=(const Ptr<const FunctionInterface>& fctPtr)
        {
            _fctPtr = fctPtr;
            return *this;
        }
        
        const inline Ptr<const FunctionInterface>& getPtr() const
        {
            return _fctPtr;
        }
    
        virtual double getValue() const
        {
            return _fctPtr->getValue();
        }
        
        virtual double getDerivative(const Ptr<Parameter>& parameter) const
        {
            return _fctPtr->getDerivative(parameter);
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            return _fctPtr->getValueAndDerivatives(parameters);
        }
       
};



}

#endif
