#include "milan/core/Prior.hh"
#include "milan/core/Function.hh"

#include "gtest/gtest.h"

TEST(Prior, construction)
{
    using namespace milan;
    Parameter mu("mu",1);
    //Function nll = (mu.ref()-1)*0.5/0.1;
    
    //Prior p1(nll);

}
