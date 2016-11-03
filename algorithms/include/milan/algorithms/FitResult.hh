#ifndef __MILAN_ALGORITHMS_FITRESULT_H__
#define __MILAN_ALGORITHMS_FITRESULT_H__

#include "milan/core/Types.hh"

#include <string>
#include <limits>
#include <unordered_map>

namespace milan
{

class FitResult
{
    protected:
        struct Hash
        {
            sizetype operator()(const std::pair<std::string,std::string>& pair) const
            {
                //make symmetric mapping but do not map all diagonal elements onto the same hash
                sizetype h1 = std::hash<std::string>()(pair.first);
                sizetype h2 = std::hash<std::string>()(pair.second);
                if (h1>h2) return h1^(h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
                else       return h2^(h1 + 0x9e3779b9 + (h2 << 6) + (h2 >> 2));
            }
        };
    
        std::unordered_map<std::string,double> _means;
        std::unordered_map<std::pair<std::string,std::string>,double,FitResult::Hash> _covariances;
        
    public:
        FitResult()
        {
        }
        
        inline void setMean(const Parameter& p, double mean)
        {
            _means[p.getName()]=mean;
        }
        
        inline void setCovariance(const Parameter& p1, const Parameter& p2, double covariance)
        {
            _covariances[std::make_pair(p1.getName(),p2.getName())]=covariance;
        }
        
        double getMean(const Parameter& p) const
        {
            auto elem = _means.find(p.getName());
            if (elem==_means.cend()) 
            {
                milan_throw("Mean of parameter with name '",p.getName(),"' not found in fit result");
            }
            return elem->second;
        }
        
        double getCovariance(const Parameter& p1, const Parameter& p2) const
        {
            auto elem = _covariances.find(std::make_pair(p1.getName(),p2.getName()));
            if (elem==_covariances.cend()) 
            {
                milan_throw("Covariance between parameters with name '",p1.getName(),"' and '",p2.getName(),"' not found in fit result");
            }
            return elem->second;
        }
};

}

#endif
