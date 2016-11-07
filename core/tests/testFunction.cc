#include "milan/core/Function.hh"

#include "gtest/gtest.h"

TEST(Function, construction)
{
    using namespace milan;
    Parameter p1("mu");
    Function fct1 = p1.ref();

}
