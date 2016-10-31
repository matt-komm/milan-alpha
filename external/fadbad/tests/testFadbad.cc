#include "fadbad/fadiff.h"
#include "gtest/gtest.h"

#include <iostream>

typedef fadbad::F<double> Ftype;


Ftype myFct(std::vector<Ftype>& x)
{
    return x[0]*x[0]+3*x[1];
}

TEST(fadbad,diff)
{
    std::vector<Ftype> x(3);
    x[0]=3;
    x[1]=2;
    x[2]=3;
    
    x[0].diff(0,1);

    Ftype res = myFct(x);
    std::cout<<"value="<<res.x()<<", diff="<<res.d(0)<<std::endl;
}

