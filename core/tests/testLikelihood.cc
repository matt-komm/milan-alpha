#include "milan/core/Likelihood.hh"
#include "milan/core/BinnedLikelihood.hh"
#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"

TEST(BinnedLikelihood, create)
{
    using namespace milan;
    
    Histogram prediction1({Binning(1,-1,1)});
    prediction1.setContent(1,1.0);
    prediction1.setError2(1,0.0);
    
    Histogram data1({Binning(1,-1,1)});
    data1.setContent(1,2.0);
    data1.setError2(1,1.0);
    
    BinnedLikelihood bll1(data1.ref(),prediction1.ref());
    
    Likelihood ll1 = bll1.ref();
    ll1.getNLL();
    
    Histogram prediction2({Binning(1,-1,1)});
    prediction2.setContent(1,1.0);
    prediction2.setError2(1,0.0);
    
    Histogram data2({Binning(1,-1,1)});
    data2.setContent(1,2.0);
    data2.setError2(1,1.0);
    
    Likelihood ll2 = BinnedLikelihood(data2,prediction2.ref());
    
    Likelihood ll3 = ll1*ll2;
    
    ll3.getNLL();
    
}

