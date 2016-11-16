#include "milan/core/ParametricHistogram.hh"
#include "milan/core/Histogram.hh"

#include "gtest/gtest.h"


#include <limits>

TEST(ParametricHistogram, create)
{
    using namespace milan;
    
    for (sizetype i = 1; i < 10; ++i)
    {
        for (sizetype v = 0; v < 10; ++v)
        {
            ParametricHistogram pHist({Binning(i,-1,1)},"signal",0.1*v);
            EXPECT_EQ(pHist.size(),i+2);

            for (sizetype ibin = 0; ibin < i+2; ++ibin)
            {
                EXPECT_DOUBLE_EQ(pHist.getContent(ibin),0.1*v);
                EXPECT_DOUBLE_EQ(pHist.getContent({ibin}),0.1*v);
                EXPECT_DOUBLE_EQ(pHist.getError({ibin}),0.0);
                EXPECT_DOUBLE_EQ(pHist.getError2(ibin),0.0);
                EXPECT_DOUBLE_EQ(pHist.getError2({ibin}),0.0);
            }
        }
    }
}

TEST(ParametricHistogram, createWithVector)
{
    using namespace milan;
    
    for (sizetype i = 1; i < 10; ++i)
    {
        for (sizetype v = 0; v < 10; ++v)
        {
            std::vector<Ptr<Parameter>> parameters(v,Ptr<Parameter>(PtrStorage::OWN,new Parameter("test"+std::to_string(v),v*0.1+0.11*i)));
            if ((i+2)==v)
            {
                ParametricHistogram pHist({Binning(i,-1,1)},parameters);
                for (sizetype ibin = 0; ibin < i+2; ++ibin)
                {
                    EXPECT_DOUBLE_EQ(pHist.getContent(ibin),0.1*v+0.11*i);
                    EXPECT_DOUBLE_EQ(pHist.getContent({ibin}),0.1*v+0.11*i);
                    EXPECT_DOUBLE_EQ(pHist.getError({ibin}),0.0);
                    EXPECT_DOUBLE_EQ(pHist.getError2(ibin),0.0);
                    EXPECT_DOUBLE_EQ(pHist.getError2({ibin}),0.0);
                }
            }
            else
            {   
                EXPECT_ANY_THROW(ParametricHistogram({Binning(i,-1,1)},parameters));
            }
        }
    }
}

TEST(ParametricHistogram, createWithVector2D)
{
    using namespace milan;
    
    for (sizetype i = 1; i < 5; ++i)
    {
        for (sizetype j = 1; j < 5; ++j)
        {
            for (sizetype v = 0; v < 30; ++v)
            {
                std::vector<Ptr<Parameter>> parameters(v,Ptr<Parameter>(PtrStorage::OWN,new Parameter("test"+std::to_string(v),v*0.1+i*0.11+j*0.111)));
                
                if ((i+2)*(j+2)==v)
                {
                    ParametricHistogram pHist({Binning(i,-1,1),Binning(j,-1,1)},parameters);
                    for (sizetype ibin = 0; ibin < pHist.size(); ++ibin)
                    {
                        EXPECT_DOUBLE_EQ(pHist.getContent(ibin),v*0.1+i*0.11+j*0.111);
                        EXPECT_DOUBLE_EQ(pHist.getContent({ibin}),v*0.1+i*0.11+j*0.111);
                        EXPECT_DOUBLE_EQ(pHist.getError({ibin}),0.0);
                        EXPECT_DOUBLE_EQ(pHist.getError2(ibin),0.0);
                        EXPECT_DOUBLE_EQ(pHist.getError2({ibin}),0.0);
                    }
                }
                else
                {   
                    EXPECT_ANY_THROW(ParametricHistogram({Binning(i,-1,1),Binning(j,-1,1)},parameters));
                }
            }
        }
    }
}

TEST(ParametricHistogram, value)
{
    using namespace milan;
    
    for (sizetype v = 0; v < 10; ++v)
    {
        ParametricHistogram pHist({Binning(10,-1,1)},"signal",0.1*v);
        EXPECT_EQ(pHist.size(),12);

        for (sizetype ibin = 0; ibin < 12; ++ibin)
        {
            EXPECT_DOUBLE_EQ(pHist.getContent(ibin),0.1*v);
            EXPECT_DOUBLE_EQ(pHist.getContent({ibin}),0.1*v);
            EXPECT_DOUBLE_EQ(pHist.getError({ibin}),0.0);
            EXPECT_DOUBLE_EQ(pHist.getError2(ibin),0.0);
            EXPECT_DOUBLE_EQ(pHist.getError2({ibin}),0.0);
            
            pHist.setContent(ibin,0.2*v+ibin*0.1);
        }

        for (sizetype ibin = 0; ibin < 12; ++ibin)
        {
            EXPECT_DOUBLE_EQ(pHist.getContent(ibin),0.2*v+ibin*0.1);
            EXPECT_DOUBLE_EQ(pHist.getContent({ibin}),0.2*v+ibin*0.1);
            EXPECT_DOUBLE_EQ(pHist.getError({ibin}),0.0);
            EXPECT_DOUBLE_EQ(pHist.getError2(ibin),0.0);
            EXPECT_DOUBLE_EQ(pHist.getError2({ibin}),0.0);   
        }
    }
}

TEST(ParametricHistogram, add)
{
    using namespace milan;
    
    for (sizetype i = 1; i < 10; ++i)
    {
        ParametricHistogram pHist1({Binning(i,-1,1)},"signal");
        HistogramFunction fctHist1(pHist1.ref());
        
        ParametricHistogram pHist2({Binning(i,-1,1)},"background1");
        HistogramFunction fctHist2(pHist2.ref());
        
        Histogram nHist3({Binning(i,-1,1)});
        HistogramFunction fctHist3(nHist3.ref());
        
        for (sizetype ibin = 0; ibin < i+2; ++ibin)
        {
            pHist1.setContent(ibin,0.1*i+0.11*ibin);
            pHist1.setContent(ibin,0.21*i-0.31*ibin);
            nHist3.setContent(ibin,0.31*i+0.21*ibin);
            nHist3.setError2(ibin,0.2*i+0.31*ibin);
        }
        HistogramFunction fctSB1 = fctHist1+fctHist2;
        HistogramFunction fctSB2 = fctHist2+fctHist3;
        HistogramFunction fctSB3 = fctHist1+fctHist2+fctHist3;
        HistogramFunction fctSB4 = fctHist1+fctHist3+fctHist2;
        
        for (sizetype ibin = 0; ibin < i+2; ++ibin)
        {
            EXPECT_DOUBLE_EQ(fctSB1.getContent(ibin),pHist1.getContent(ibin)+pHist2.getContent(ibin));
            EXPECT_DOUBLE_EQ(fctSB2.getContent(ibin),pHist2.getContent(ibin)+nHist3.getContent(ibin));
            EXPECT_DOUBLE_EQ(fctSB3.getContent(ibin),pHist1.getContent(ibin)+pHist2.getContent(ibin)+nHist3.getContent(ibin));
            EXPECT_DOUBLE_EQ(fctSB4.getContent(ibin),pHist1.getContent(ibin)+pHist2.getContent(ibin)+nHist3.getContent(ibin));
            
            EXPECT_DOUBLE_EQ(fctSB1.getError2(ibin),0.0);
            EXPECT_DOUBLE_EQ(fctSB2.getError2(ibin),nHist3.getError2(ibin));
            EXPECT_DOUBLE_EQ(fctSB3.getError2(ibin),nHist3.getError2(ibin));
            EXPECT_DOUBLE_EQ(fctSB4.getError2(ibin),nHist3.getError2(ibin));
        }
    }
}

TEST(ParametricHistogram, derivative)
{
    using namespace milan;
    ParametricHistogram pHist({Binning(10,-1,1)},"signal");
    const std::vector<Ptr<Parameter>>& parameters = pHist.getParameters();
    EXPECT_EQ(pHist.size(),parameters.size());
    for (sizetype i = 0; i < pHist.size(); ++i)
    {
        for (sizetype j = 0; j < pHist.size(); ++j)
        {
            EXPECT_DOUBLE_EQ(pHist.getDerivative(i,parameters[j]),i==j?1:0);
        } 
    }
}

