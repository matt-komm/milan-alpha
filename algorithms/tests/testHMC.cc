#include "milan/algorithms/HMC.hh"

#include "milan/core/Histogram.hh"
#include "milan/core/HistogramFunction.hh"
#include "milan/core/Parameter.hh"
#include "milan/core/BinnedLikelihood.hh"

#include "gtest/gtest.h"
/*
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TColor.h"
#include "Rtypes.h"
#include "TF1.h"
*/
TEST(HMC, construction)
{
    using namespace milan;
    
    HMC hmc;
    Parameter p1("signal");
    Parameter p2("background");
    Parameter p3("background");
    
    EXPECT_EQ(hmc.getParameters().size(),0);
    hmc.addParameter(p1);
    EXPECT_EQ(hmc.getParameters().size(),1);
    EXPECT_ANY_THROW(hmc.addParameter(p1));
    EXPECT_EQ(hmc.getParameters().size(),1);
    hmc.addParameter(p2);
    EXPECT_EQ(hmc.getParameters().size(),2);
    EXPECT_ANY_THROW(hmc.addParameter(p3));
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
    HistogramFunction signalPrediction = HistogramFunction(signalTemplate.ref())*signalYield.ref();
    
    BinnedLikelihood bll(dataFct,signalPrediction);
    
    //Likelihood ll = SimpleGaussianLL(signalYield,3.0,2.0);
    Likelihood ll = SimplePoissonLL(signalYield,2.0);
    //Likelihood ll = bll.copy();
    
    double* x = new double[40];
    double* nll = new double[40];
    double sum = 0.0;
    /*
    for (sizetype i = 0; i < 40; ++i)
    {
        signalYield.setValue(0.0+0.2*i);
        printf(
            "nll(s=%2.1f) = %+5.3f\n"
            signalYield.getValue(),
            ll.getNLL()
        );
        x[i]=signalYield.getValue();
        nll[i]=std::exp(-ll.getNLL());
        sum+=nll[i];
    }
    */
    //TH1F hist("hist","",40,0,8);
    
    
    HMC hmc;
    hmc.addParameter(signalYield);
    std::vector<Ptr<Parameter>> bbParameters = ll.getLagrangeParameters();
    //for (unsigned int i = 0; i < bbParameters.size(); ++i) hmc.addParameter(bbParameters[i]);
    const sizetype BURN = 100;
    const sizetype TOYS = 10000;
    std::vector<double> posterior(TOYS);
    
    int accepted = 0;
    double mean = 0.0;
    double mean2 = 0.0;
    
    //mean is the value for signalYield that minimizes nll!;
    double nllMin = ll.getNLL();
    double signalYieldMin = 0.0;
    
    for (unsigned int i = 0; i < BURN; ++i)
    {
        hmc.step(ll.getPtr(),10);
    }
    for (unsigned int i = 0; i < TOYS; ++i)
    {
        if (hmc.step(ll.getPtr(),10)) accepted++;
        mean+=signalYield.getValue();
        mean2+=signalYield.getValue()*signalYield.getValue();
        //hist.Fill(signalYield.getValue());
        posterior[i]=signalYield.getValue();
        if (nllMin>ll.getNLL())
        {
            nllMin = ll.getNLL();
            signalYieldMin = signalYield.getValue();
        }
    }
    std::sort(posterior.begin(), posterior.end());
    
    
    
    
    std::cout<<"eff="<<1.0*accepted/TOYS<<std::endl;
    std::cout<<"mean="<<mean/TOYS<<", rms="<<std::sqrt(mean2/TOYS-mean*mean/TOYS/TOYS)<<std::endl;
    std::cout<<"median="<<posterior[TOYS/2]<<" ["<<posterior[(sizetype)TOYS*0.84]<<"; "<<posterior[(sizetype)TOYS*0.16]<<"]"<<std::endl;
    std::cout<<"signalYield@nllmax="<<signalYieldMin<<std::endl;
    /*
    TCanvas cv("cv","",800,600);
    hist.Scale(sum/hist.Integral());
    hist.SetLineWidth(2);
    hist.SetLineColor(kBlue-3);
    hist.Draw();
    TGraph g(40,x,nll);
    g.SetLineWidth(2);
    g.SetLineColor(kGreen+1);
    g.SetLineStyle(2);
    g.Draw("LP");
    
    TF1 tf("tf","[0]*TMath::Poisson(2,x)",0,8);
    tf.SetParameter(0,sum/40*8);
    tf.Draw("Same");
    
    cv.Print("hist.pdf");
    */
    delete [] x;
    delete [] nll;
    
}
