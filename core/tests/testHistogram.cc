#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"

TEST(Histogram, construction)
{
    using namespace milan;
    for (unsigned int i = 0; i<100000; ++i)
    {
        Histogram hist(10000);
        hist*=5.0f;
    }
    //EXPECT_EQ(0,0);
}
/*
int main()
{
    using namespace milan;
    for (unsigned int i = 0; i<100000; ++i)
    {
        Histogram hist(10000);
        hist*=5.0f;
    }
    return 0;
}
*/
