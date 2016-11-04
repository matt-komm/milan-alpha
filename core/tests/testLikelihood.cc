#include "milan/core/Likelihood.hh"
#include "milan/core/BinnedLikelihood.hh"
#include "milan/core/Histogram.hh"

#include "vdt/vdtMath.h"
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
    
    Histogram data_fake({Binning(2,-1,1)});
    data_fake.setContent(1,2.0);
    data_fake.setError2(1,1.0);
    
    BinnedLikelihood bll1(data1.ref(),prediction1.ref());
    
    EXPECT_ANY_THROW(BinnedLikelihood(data_fake,prediction1));
    
    Likelihood ll1 = bll1.ref();
    EXPECT_EQ(ll1.getLagrangeParameters().size(),prediction1.size());
    ll1.getNLL();
    
    Histogram prediction2({Binning(10,-1,1)});
    prediction2.setContent(1,1.0);
    prediction2.setError2(1,0.0);
    
    Histogram data2({Binning(10,-1,1)});
    data2.setContent(1,2.0);
    data2.setError2(1,1.0);
    
    
    Likelihood ll2 = BinnedLikelihood(data2,prediction2.ref());
    EXPECT_EQ(ll2.getLagrangeParameters().size(),prediction2.size());
    
    Histogram prediction3({Binning(5,-1,1),Binning(10,-1,1)});
    prediction3.setContent(1,1.0);
    prediction3.setError2(1,0.0);
    
    Histogram data3({Binning(5,-1,1),Binning(10,-1,1)});
    data3.setContent(1,2.0);
    data3.setError2(1,1.0);
    Likelihood ll3 = BinnedLikelihood(data3,prediction3.ref()); 
    EXPECT_EQ(ll3.getLagrangeParameters().size(),prediction3.size());
    
    Likelihood ll_tot1 = ll1*ll2;
    EXPECT_EQ(ll_tot1.getLagrangeParameters().size(),ll1.getLagrangeParameters().size()+ll2.getLagrangeParameters().size());
    
    Likelihood ll_tot2 = ll1*ll2*ll3;
    EXPECT_EQ(ll_tot2.getLagrangeParameters().size(),ll1.getLagrangeParameters().size()+ll2.getLagrangeParameters().size()+ll3.getLagrangeParameters().size());
}
    
void speed(unsigned int bins)
{    
    using namespace milan;
    Parameter signalStrength("mu",1.0);
    
    Histogram signalTemplate({Binning(bins,-1,1)});
    HistogramFunction signalPrediction = HistogramFunction(signalTemplate.ref())*signalStrength;
    Histogram data1({Binning(bins,-1,1)});
    Likelihood ll1 = BinnedLikelihood(data1.ref(),signalPrediction);
    
    Parameter backgroundStrength("bg",1.0);
    
    Histogram backgroundTemplate({Binning(bins,-1,1)});
    HistogramFunction backgroundPrediction = HistogramFunction(backgroundTemplate.ref())*backgroundStrength;
    Histogram data2({Binning(bins,-1,1)});
    Likelihood ll2 = BinnedLikelihood(data2.ref(),backgroundPrediction);
    
    Histogram prediction3({Binning(bins,-1,1)});
    Histogram data3({Binning(bins,-1,1)});
    Likelihood ll3 = BinnedLikelihood(data3.ref(),prediction3.ref());
    
    for (unsigned int i = 0; i < bins; ++i)
    {
        signalTemplate.setContent(i,3.0+i*0.1);
        signalTemplate.setError2(i,1.5+std::sqrt(i*0.025));
        
        data1.setContent(i,2.0+i*0.1);
        data1.setError2(i,std::sqrt(2.0+i*0.1));
        
        
        backgroundTemplate.setContent(i,3.0+i*0.25);
        backgroundTemplate.setError2(i,1.5+std::sqrt(i*0.25));
        
        data2.setContent(i,1.5+i*0.2);
        data2.setError2(i,std::sqrt(2.0+i*0.01));
        
        
        prediction3.setContent(i,3.0+i*0.01);
        prediction3.setError2(i,1.1+std::sqrt(i*0.01));
        
        data3.setContent(i,3.5-1.*i/bins);
        data3.setError2(i,std::sqrt(1.*i/bins));
    }
    
    
    
    Likelihood tot_ll = ll1*ll2*ll3;
    std::vector<Ptr<Parameter>> bbParameters = tot_ll.getLagrangeParameters();
    std::cout<<"N(bb)="<<bbParameters.size()<<std::endl;
    
    std::cout<<"testing "<<bins<<" ...";
    double sum = 0.0;
    for (unsigned int toy = 0; toy < 10000; ++toy)
    {
        //std::vector<double> res = tot_ll.getNLLValueAndDerivatives({signalStrength,backgroundStrength});
        //double x = res[0]+res[1]+res[2];
        
        std::vector<double> res = tot_ll.getNLLValueAndDerivatives(bbParameters);
        double x = res[0]+res[1];
        /*
        double x = tot_ll.getNLL();
        x+=tot_ll.getNLLDerivative(signalStrength);    
        x+=tot_ll.getNLLDerivative(backgroundStrength);   
        */
         
        sum+=x;
       
    }
    std::cout<<"done ("<<sum<<")"<<std::endl;
}
/*
TEST(BinnedLikelihood, speed20)
{
    speed(20);
}

TEST(BinnedLikelihood, speed40)
{
    speed(40);
}

TEST(BinnedLikelihood, speed80)
{
    speed(80);
}

TEST(BinnedLikelihood, speed160)
{
    speed(160);
}

TEST(BinnedLikelihood, speed320)
{
    speed(320);
}
*/


TEST(BinnedLikelihood, counting)
{
    using namespace milan;
    
    Histogram prediction({Binning(1,-1,1)});
    prediction.setError2(0,0.0);
    
    Histogram data({Binning(1,-1,1)});
    data.setError2(0,0.0);
    
    BinnedLikelihood bll(data.ref(),prediction.ref());
    
    Likelihood ll = bll.ref();
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    
    for (unsigned int i = 0; i < 10; ++i)
    {
        for (unsigned int j = 0; j < 10; ++j)
        {
            for (unsigned int k = 0; k < 10; ++k)
            {
                for (unsigned int l = 0; l < 10; ++l)
                {
                    Ptr<Parameter> bb = bbParameters[1];
                    
                    const double e = k*0.1;
                    const double d = 1.0*j;
                    const double b = -2.0+0.2*l;
                    const double raw_p = 1.3*i;
                    const double s = 1.;
                    const double p = raw_p*s+b*std::fabs(e*s);
                    
                    prediction.setContent(1,raw_p);
                    prediction.setError2(1,e*e);
                    data.setContent(1,d);
                    bb->setValue(b);
                    
                    const double nll = ll.getNLL();
                    const double nll_alt = ll.getNLLValueAndDerivatives({})[0];
                    
                    const double num_nll = -d*vdt::fast_log(p)+p+0.5*b*b;
                    
                    if (d<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_DOUBLE_EQ(nll,0.0);
                        EXPECT_DOUBLE_EQ(nll_alt,0.0);
                    }
                    else if (std::isnan(num_nll) || p<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_TRUE(std::isnan(nll));
                        EXPECT_TRUE(std::isnan(nll_alt));
                    }
                    else
                    {
                        EXPECT_DOUBLE_EQ(nll,num_nll);
                        EXPECT_DOUBLE_EQ(nll_alt,num_nll);
                    }
                }
            }
        }
    }
}


TEST(BinnedLikelihood, withParameter)
{
    using namespace milan;
    
    Parameter strength("mu",1.0);
    
    Histogram nominalSignal({Binning(1,-1,1)});
    nominalSignal.setError2(0,1.0);
    
    HistogramFunction prediction = HistogramFunction(nominalSignal.ref())*strength;
    
    Histogram data({Binning(1,-1,1)});
    data.setError2(0,1.0);
    
    Likelihood ll = BinnedLikelihood(data.ref(),prediction);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    
    for (unsigned int i = 0; i < 10; ++i)
    {
        for (unsigned int j = 0; j < 10; ++j)
        {
            for (unsigned int k = 0; k < 10; ++k)
            {
                for (unsigned int l = 0; l < 10; ++l)
                {
                    Ptr<Parameter> bb = bbParameters[1];
                    
                    const double e = k*0.1;
                    const double d = 1.0*j;
                    const double b = -2.0+0.2*l;
                    const double raw_p = 1.3*i;
                    const double s = d*0.1+0.3*e+std::fabs(b);
                    const double p = raw_p*s+b*std::fabs(e*s);
                    
                    nominalSignal.setContent(1,raw_p);
                    nominalSignal.setError2(1,e*e);
                    data.setContent(1,d);
                    bb->setValue(b);
                    strength.setValue(s);
                    
                    const double nll = ll.getNLL();
                    const double nll_alt = ll.getNLLValueAndDerivatives({})[0];
                    
                    const double num_nll = -d*vdt::fast_log(p)+p+0.5*b*b;
                    
                    if (d<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_DOUBLE_EQ(nll,0.0);
                        EXPECT_DOUBLE_EQ(nll_alt,0.0);
                    }
                    else if (std::isnan(num_nll) || p<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_TRUE(std::isnan(nll)) << "Value is "<<nll<<" prediction="<<p;
                        EXPECT_TRUE(std::isnan(nll_alt)) << "Value is "<<nll<<" prediction="<<p;
                    }
                    else
                    {
                        EXPECT_FLOAT_EQ(nll,num_nll);
                        EXPECT_FLOAT_EQ(nll_alt,num_nll);
                    }
                }
            }
        }
    }
}

TEST(BinnedLikelihood, diffParameter)
{
    using namespace milan;
    
    Parameter strength("mu",1.0);
    
    Histogram nominalSignal({Binning(1,-1,1)});
    nominalSignal.setError2(1,1.0);
    
    HistogramFunction prediction = HistogramFunction(nominalSignal.ref())*strength;
    
    Histogram data({Binning(1,-1,1)});
    
    Likelihood ll = BinnedLikelihood(data.ref(),prediction);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    
    for (unsigned int i = 0; i < 10; ++i)
    {
        for (unsigned int j = 0; j < 10; ++j)
        {
            for (unsigned int k = 0; k < 10; ++k)
            {
                for (unsigned int l = 0; l < 10; ++l)
                {
                    Ptr<Parameter> bb = bbParameters[1];
                    
                    const double e = k*0.1;
                    const double d = 1.0*j;
                    const double b = -2.0+0.2*l;
                    const double raw_p = 1.3*i;
                    const double s = d*0.1+0.3*e+std::fabs(b);
                    const double p = raw_p*s+b*std::fabs(e*s);
                    
                    nominalSignal.setContent(1,raw_p);
                    nominalSignal.setError2(1,e*e);
                    data.setContent(1,d);
                    bb->setValue(b);
                    strength.setValue(s);
                    
                    double diffNll_analytical = ll.getNLLDerivative(strength);
                    double diffNll_analytical_alt = ll.getNLLValueAndDerivatives({strength})[1];
                    
                    const double diffNll_numerical = -d*raw_p/p+raw_p;
                    
                    if (d<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_DOUBLE_EQ(diffNll_analytical,0.0);
                        EXPECT_DOUBLE_EQ(diffNll_analytical_alt,0.0);
                    }
                    else if (std::isnan(diffNll_numerical) || p<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_TRUE(std::isnan(diffNll_analytical)) << "Value is "<<diffNll_analytical<<" prediction="<<p;
                        EXPECT_TRUE(std::isnan(diffNll_analytical_alt)) << "Value is "<<diffNll_analytical_alt<<" prediction="<<p;
                    }
                    else
                    {
                        EXPECT_FLOAT_EQ(diffNll_analytical,diffNll_numerical);
                        EXPECT_FLOAT_EQ(diffNll_analytical_alt,diffNll_numerical);
                    }
                }
            }
        }
    }
}

TEST(BinnedLikelihood, diffBB)
{
    using namespace milan;
    
    Parameter strength("mu",1.0);
    
    Histogram nominalSignal({Binning(1,-1,1)});
    nominalSignal.setError2(1,1.0);
    
    HistogramFunction prediction = HistogramFunction(nominalSignal.ref())*strength;
    
    Histogram data({Binning(1,-1,1)});
    
    Likelihood ll = BinnedLikelihood(data.ref(),prediction);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    
    for (unsigned int i = 0; i < 10; ++i)
    {
        for (unsigned int j = 0; j < 10; ++j)
        {
            for (unsigned int k = 0; k < 10; ++k)
            {
                for (unsigned int l = 0; l < 10; ++l)
                {
                    Ptr<Parameter> bb = bbParameters[1];
                    
                    const double e = k*0.1;
                    const double d = 1.0*j;
                    const double b = -2.0+0.2*l;
                    const double raw_p = 1.3*i;
                    const double s = d*0.1+0.3*e+std::fabs(b);
                    const double p = raw_p*s+b*std::fabs(e*s);
                    
                    nominalSignal.setContent(1,raw_p);
                    nominalSignal.setError2(1,e*e);
                    data.setContent(1,d);
                    bb->setValue(b);
                    strength.setValue(s);
                    
                    double diffNll_analytical = ll.getNLLDerivative(bb);
                    double diffNll_analytical_alt = ll.getNLLValueAndDerivatives({bb})[1];
                    
                    //special case if e == 0 then BB is deactivated
                    const double diffNll_numerical = e<std::numeric_limits<double>::epsilon()?0:-d*e*s/p+e*s-b;
                    
                    if (d<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_DOUBLE_EQ(diffNll_analytical,0.0);
                        EXPECT_DOUBLE_EQ(diffNll_analytical_alt,0.0);
                    }
                    else if (std::isnan(diffNll_numerical) || p<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_TRUE(std::isnan(diffNll_analytical)) << "Value is "<<diffNll_analytical<<" prediction="<<p;
                        EXPECT_TRUE(std::isnan(diffNll_analytical_alt)) << "Value is "<<diffNll_analytical_alt<<" prediction="<<p;
                    }
                    else
                    {
                        EXPECT_FLOAT_EQ(diffNll_analytical,diffNll_numerical);
                        EXPECT_FLOAT_EQ(diffNll_analytical_alt,diffNll_numerical);
                    }
                }
            }
        }
    }
}

