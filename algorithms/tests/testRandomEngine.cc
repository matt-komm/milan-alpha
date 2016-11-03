#include "milan/algorithms/RandomEngine.hh"

#include "gtest/gtest.h"

#include <iostream>

TEST(RandomEngine, randomness)
{
    using namespace milan;
    
    RandomEngine randomEngine(123);
    std::vector<sizetype> quantiles(10,0);
    for (sizetype i = 0; i < 100000; ++i)
    {
        const sizetype index = 1.0*randomEngine()/std::numeric_limits<uint64>::max()*quantiles.size();
        quantiles[index]++;
    }
    for (sizetype i = 0; i < quantiles.size(); ++i)
    {
        std::cout<<quantiles[i]<<std::endl;
    }
}
