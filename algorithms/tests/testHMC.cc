#include "milan/algorithms/HMC.hh"

#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/BinnedLikelihood.hh"

#include "gtest/gtest.h"

TEST(HMC, construction)
{
    using namespace milan;
    
    HMC hmc;
    Parameter p1("signal");
    Parameter p2("background");
    Parameter p3("background");
    
    EXPECT_EQ(hmc.getParameters().size(),0);
    hmc.addParameter(p1.ref());
    EXPECT_EQ(hmc.getParameters().size(),1);
    EXPECT_ANY_THROW(hmc.addParameter(p1.ref()));
    EXPECT_EQ(hmc.getParameters().size(),1);
    hmc.addParameter(p2.ref());
    EXPECT_EQ(hmc.getParameters().size(),2);
    EXPECT_ANY_THROW(hmc.addParameter(p3.ref()));
    EXPECT_EQ(hmc.getParameters().size(),2);
}

class SimpleLL:
    public milan::LikelihoodInterface,
    public milan::PtrInterface<milan::LikelihoodInterface,SimpleLL>
{
    protected:
        milan::Ptr<milan::Parameter> _p;
        double _mean;
        double _std;
        std::vector<milan::Ptr<milan::Parameter>> _lagrangeParameters;
    public:
    
        SimpleLL(const milan::Ptr<milan::Parameter>& p, double mean=0, double std=1): 
            _p(p),
            _mean(mean),
            _std(std),
            _lagrangeParameters(0) 
        {
        }
        
        virtual double getNLLDerivative(const milan::Ptr<milan::Parameter>& parameter) const
        {
            if (*_p == *parameter) return (_p->getValue()-_mean)/_std/_std;
            return 0;
        }
        virtual double getNLL() const
        {
            return 0.5*(_p->getValue()-_mean)*(_p->getValue()-_mean)/_std/_std; //Gaussian with mean=1.5 and std=2
        }
        virtual std::vector<double> getNLLValueAndDerivatives(const std::vector<milan::Ptr<milan::Parameter>>& parameters) const
        {
            std::vector<double> result(parameters.size()+1,0);
            result[0] = getNLL();
            for (milan::sizetype i = 0; i < parameters.size(); ++i) result[i+1] = getNLLDerivative(parameters[i]);
            return result;
        }
        
        //returns pointers to potential lagrange parameters (e.g. for Barlow-Beeston) for additional minimizations
        virtual const std::vector<milan::Ptr<milan::Parameter>>& getLagrangeParameters() const
        {
            return _lagrangeParameters;

        }
        
        operator milan::Likelihood()
        {
            return copy();
        }
        
};

TEST(HMC, speed)
{
    using namespace milan;
    
    const sizetype N = 1;
    Histogram data({Binning(N,-1,1)});
    for (unsigned int i = 0; i < N; ++i) data.setContent(i+1,1);
    HistogramFunction dataFct(data.ref());
    
    Parameter signalYield("mu",1.0,Parameter::MIN,Parameter::MAX,0.2);
    Histogram signalTemplate({Binning(N,-1,1)});
    for (unsigned int i = 0; i < N; ++i) signalTemplate.setContent(i+1,1);
    for (unsigned int i = 0; i < N; ++i) signalTemplate.setError2(i+1,0.0);
    HistogramFunction signalPrediction = HistogramFunction(signalTemplate.ref())*signalYield;
    
    BinnedLikelihood bll(dataFct,signalPrediction);
    
    //Likelihood ll = SimpleLL(signalYield,1.3,0.3);
    Likelihood ll = bll.copy();
    for (sizetype i = 0; i < 21; ++i)
    {
        signalYield.setValue(0.0+0.1*i);
        std::cout<<"nll(s="<<signalYield.getValue()<<")="<<ll.getNLL()<<", d1="<<data.getContent(1)<<", s1="<<signalTemplate.getContent(1)<<", p1="<<signalPrediction.getContent(1)<<std::endl;
    }
    
    HMC hmc;
    hmc.addParameter(signalYield);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    //for (unsigned int i = 0; i < bbParameters.size(); ++i) hmc.addParameter(bbParameters[i]);
    const sizetype BURN = 1000;
    const sizetype TOYS = 10000;
    int accepted = 0;
    double mean = 0.0;
    double mean2 = 0.0;
    for (unsigned int i = 0; i < BURN; ++i)
    {
        hmc.step(ll.getPtr(),10);
    }
    for (unsigned int i = 0; i < TOYS; ++i)
    {
        if (hmc.step(ll.getPtr(),10)) accepted++;
        mean+=signalYield.getValue();
        mean2+=signalYield.getValue()*signalYield.getValue();
    }
    std::cout<<"eff="<<1.0*accepted/TOYS<<std::endl;
    std::cout<<"mean="<<mean/TOYS<<std::endl;
    std::cout<<"rms="<<std::sqrt(mean2/TOYS-mean*mean/TOYS/TOYS)<<std::endl;
}
