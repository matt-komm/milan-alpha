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

class SimpleGaussianLL:
    public milan::LikelihoodInterface,
    public milan::PtrInterface<milan::LikelihoodInterface,SimpleGaussianLL>
{
    protected:
        milan::Ptr<milan::Parameter> _p;
        double _mean;
        double _std;
        std::vector<milan::Ptr<milan::Parameter>> _lagrangeParameters;
    public:
    
        SimpleGaussianLL(const milan::Ptr<milan::Parameter>& p, double mean=0, double std=1): 
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
            return 0.5*(_p->getValue()-_mean)*(_p->getValue()-_mean)/_std/_std; //Gaussian with mean=p
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

class SimplePoissonLL:
    public milan::LikelihoodInterface,
    public milan::PtrInterface<milan::LikelihoodInterface,SimplePoissonLL>
{
    protected:
        milan::Ptr<milan::Parameter> _p;
        double _obs;
        std::vector<milan::Ptr<milan::Parameter>> _lagrangeParameters;
    public:
    
        SimplePoissonLL(const milan::Ptr<milan::Parameter>& p, double obs=1): 
            _p(p),
            _obs(obs),
            _lagrangeParameters(0) 
        {
        }
        
        virtual double getNLLDerivative(const milan::Ptr<milan::Parameter>& parameter) const
        {
            if (*_p == *parameter) return -_obs/_p->getValue()+1;
            return 0;
        }
        virtual double getNLL() const
        {
            return -_obs*vdt::fast_log(_p->getValue())+_p->getValue(); //Poisson with mean=p
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
    for (unsigned int i = 0; i < N; ++i) data.setContent(i+1,3);
    HistogramFunction dataFct(data.ref());
    
    Parameter signalYield("mu",1.0,Parameter::MIN,Parameter::MAX,0.2);
    Histogram signalTemplate({Binning(N,-1,1)});
    for (unsigned int i = 0; i < N; ++i) signalTemplate.setContent(i+1,1);
    for (unsigned int i = 0; i < N; ++i) signalTemplate.setError2(i+1,0.0);
    HistogramFunction signalPrediction = HistogramFunction(signalTemplate.ref())*signalYield;
    
    BinnedLikelihood bll(dataFct,signalPrediction);
    
    //Likelihood ll = SimpleGaussianLL(signalYield,-3.0,2.0);
    Likelihood ll = SimplePoissonLL(signalYield,24.0);
    //Likelihood ll = bll.copy();
    for (sizetype i = 0; i < 41; ++i)
    {
        signalYield.setValue(0.0+0.2*i);
        printf(
            "nll(s=%2.1f) = %+5.3f\n",// | d=%3.1f | s=%3.1f | p=%3.1f\n",
            signalYield.getValue(),
            ll.getNLL()/*,
            data.getContent(1),
            signalTemplate.getContent(1),
            signalPrediction.getContent(1)*/
        );
    }
    
    HMC hmc;
    hmc.addParameter(signalYield);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    //for (unsigned int i = 0; i < bbParameters.size(); ++i) hmc.addParameter(bbParameters[i]);
    const sizetype BURN = 1000;
    const sizetype TOYS = 100000;
    int accepted = 0;
    double mean = 0.0;
    double mean2 = 0.0;
    for (unsigned int i = 0; i < BURN; ++i)
    {
        hmc.step(ll.getPtr(),1);
    }
    for (unsigned int i = 0; i < TOYS; ++i)
    {
        if (hmc.step(ll.getPtr(),1)) accepted++;
        mean+=signalYield.getValue();
        mean2+=signalYield.getValue()*signalYield.getValue();
    }
    std::cout<<"eff="<<1.0*accepted/TOYS<<std::endl;
    std::cout<<"mean="<<mean/TOYS<<std::endl;
    std::cout<<"rms="<<std::sqrt(mean2/TOYS-mean*mean/TOYS/TOYS)<<std::endl;
}
