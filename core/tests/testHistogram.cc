#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"

TEST(Histogram, construction)
{
    using namespace milan;
    Histogram hist(10000);
    hist*=5.0f;
}

