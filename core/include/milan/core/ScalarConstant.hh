#ifndef __MILAN_CORE_SCALARCONSTANT_H__
#define __MILAN_CORE_SCALARCONSTANT_H__

#include "milan/core/Types.hh"
#include "milan/core/FunctionInterface.hh"
#include "milan/core/Ptr.hh"

namespace milan
{

class ScalarConstant:
    public FunctionInterface,
    public PtrInterface<FunctionInterface,ScalarConstant>
{
    protected:
        double _value;
    public:
        ScalarConstant(double value):
            _value(value)
        {
        }
        
        virtual double getValue() const
        {
            return _value;
        }
        
        virtual double getDifferential(const Ptr<Parameter>&) const
        {
            return 0;
        }
        
        virtual std::vector<double> getValueAndDerivatives(const std::vector<Ptr<Parameter>>& parameters) const
        {
            std::vector<double> result(parameters.size()+1,0);
            result[0]=_value;
            return result;
        }
        
        inline ScalarConstant& operator+=(const ScalarConstant& scalar)
        {
            _value+=scalar._value;
            return *this;
        }

        inline ScalarConstant& operator-=(const ScalarConstant& scalar)
        {
            _value-=scalar._value;
            return *this;
        }
        
        inline ScalarConstant& operator*=(const ScalarConstant& scalar)
        {
            _value*=scalar._value;
            return *this;
        }
        
        inline ScalarConstant& operator/=(const ScalarConstant& scalar)
        {
            _value/=scalar._value;
            return *this;
        }
};

}

#endif
