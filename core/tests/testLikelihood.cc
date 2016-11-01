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
    
    std::cout<<"testing "<<bins<<" ...";
    double sum = 0.0;
    for (unsigned int toy = 0; toy < 10000; ++toy)
    {
        double x = tot_ll.getNLL();
        //x+=tot_ll.getNLLDerivative(signalStrength);    
        //x+=tot_ll.getNLLDerivative(backgroundStrength);    
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
    for (unsigned int i = 0; i < 20; ++i)
    {
        for (unsigned int j = 0; j < 20; ++j)
        {
            const double d = 1.0*j;
            const double p = 0.1*i;
            prediction.setContent(1,p);
            data.setContent(1,d);
            
            double nll = ll.getNLL();
            
            if (d<std::numeric_limits<double>::epsilon())
            {
                EXPECT_DOUBLE_EQ(nll,0.0);
            }
            else
            {
                EXPECT_DOUBLE_EQ(nll,d*vdt::fast_log(p)-p);
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
    
    for (unsigned int i = 0; i < 20; ++i)
    {
        for (unsigned int j = 0; j < 20; ++j)
        {
            for (unsigned int k = 0; k < 20; ++k)
            {
                const double s = k*0.1-0.1;
                const double d = 1.0*j-0.1;
                const double p = 0.1*i-0.1;
                
                strength.setValue(s);
                nominalSignal.setContent(1,p);
                data.setContent(1,d);
                
                double nll = ll.getNLL();
                double nll_alt = ll.getNLLValueAndDerivatives({})[0];
                if (d<std::numeric_limits<double>::epsilon())
                {
                    EXPECT_DOUBLE_EQ(nll,0.0);
                    EXPECT_DOUBLE_EQ(nll_alt,0.0);
                }
                else if (std::isnan(d*vdt::fast_log(p*s)-p*s))
                {
                    EXPECT_TRUE(std::isnan(nll));
                    EXPECT_TRUE(std::isnan(nll_alt));
                }
                else
                {
                    EXPECT_DOUBLE_EQ(nll,d*vdt::fast_log(p*s)-p*s);
                    EXPECT_DOUBLE_EQ(nll_alt,d*vdt::fast_log(p*s)-p*s);
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
    
    for (unsigned int i = 0; i < 20; ++i)
    {
        for (unsigned int j = 0; j < 20; ++j)
        {
            for (unsigned int k = 0; k < 20; ++k)
            {
                const double s = k*0.1-0.1;
                const double d = 1.0*j-0.1;
                const double p = 0.1*i-0.1;
                
                strength.setValue(s);
                nominalSignal.setContent(1,p);
                data.setContent(1,d);
                
                double diffNll_analytical = ll.getNLLDerivative(strength);
                double diffNll_analytical_alt = ll.getNLLValueAndDerivatives({strength})[1];
                const double h = 0.000001;
                strength.setValue(s-h);
                double nll1 = ll.getNLL();
                strength.setValue(s+h);
                double nll2 = ll.getNLL();
                
                double diffNll_numerical = (-0.5*nll1+0.5*nll2)/h;
                
                if (d<std::numeric_limits<double>::epsilon())
                {
                    EXPECT_DOUBLE_EQ(diffNll_analytical,0.0);
                    EXPECT_DOUBLE_EQ(diffNll_analytical_alt,0.0);
                }
                else if (std::isnan(diffNll_numerical))
                {
                    EXPECT_TRUE(std::isnan(diffNll_analytical))<< 
                        "analytical ("<<diffNll_analytical<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";;
                    EXPECT_TRUE(std::isnan(diffNll_analytical_alt))<< 
                        "analytical alternative ("<<diffNll_analytical_alt<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
                }
                else if (!std::isnan(diffNll_analytical) || !std::isnan(diffNll_analytical_alt))
                {
                    EXPECT_TRUE(std::fabs(diffNll_analytical-diffNll_numerical)<h)<< 
                        "analytical ("<<diffNll_analytical<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
                    EXPECT_TRUE(std::fabs(diffNll_analytical_alt-diffNll_numerical)<h)<< 
                        "analytical alternative ("<<diffNll_analytical_alt<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
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
    data.setError2(1,0.0);
    
    Likelihood ll = BinnedLikelihood(data.ref(),prediction);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    EXPECT_EQ(bbParameters.size(),(unsigned int)3);
    
    for (unsigned int i = 0; i < 2; ++i)
    {
        for (unsigned int j = 0; j < 2; ++j)
        {
            for (unsigned int k = 0; k < 2; ++k)
            {   
                for (unsigned int l = 0; l < 2; ++l)
                {   
                  
                    Parameter& bb = *bbParameters[1];
                    
                    const double s = k*0.1-0.1;
                    const double d = 1.0*j-0.1;
                    const double p = 0.1*i-0.1;
                    const double b = 0;//-2.0+0.2*l;
                    
                    strength.setValue(s);
                    nominalSignal.setContent(1,p);
                    data.setContent(1,d);
                    bb.setValue(b);
                    
                    double diffNll_analytical = ll.getNLLDerivative(bb);
                    double diffNll_analytical_alt = ll.getNLLValueAndDerivatives({bb})[1];
                    
                    const double h = 0.00001;
                    bb.setValue(b-h);
                    double nll1 = ll.getNLL();
                    
                    bb.setValue(b+h);
                    double nll2 = ll.getNLL();

                    double diffNll_numerical = (-0.5*nll1/h+0.5*nll2/h);

                    if (d<std::numeric_limits<double>::epsilon())
                    {
                        EXPECT_DOUBLE_EQ(diffNll_analytical,0.0);
                        EXPECT_DOUBLE_EQ(diffNll_analytical_alt,0.0);
                    }
                    else if (std::isnan(diffNll_numerical))
                    {
                        EXPECT_TRUE(std::isnan(diffNll_analytical))<< 
                            "analytical ("<<diffNll_analytical<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";;
                        EXPECT_TRUE(std::isnan(diffNll_analytical_alt))<< 
                            "analytical alternative ("<<diffNll_analytical_alt<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
                    }
                    else if (!std::isnan(diffNll_analytical) || !std::isnan(diffNll_analytical_alt))
                    {
                        EXPECT_TRUE(std::fabs(diffNll_analytical-diffNll_numerical)<(100.*h))<< 
                            "analytical ("<<diffNll_analytical<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
                        EXPECT_TRUE(std::fabs(diffNll_analytical_alt-diffNll_numerical)<(100.*h))<< 
                            "analytical alternative ("<<diffNll_analytical_alt<<") does not equal numerical ("<<diffNll_numerical<<" = (-0.5*"<<nll1<<"+0.5*"<<nll2<<")/"<<h<<") differentiation result";
                    }
                }
            }
        }
    }
}


