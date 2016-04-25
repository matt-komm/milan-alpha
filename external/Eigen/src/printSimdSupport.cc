#include "Eigen/Eigen"

#include <iostream>
    

int main()
{
    std::cout<<"Summary of Eigen config"<<std::endl;
    std::cout<<"-----------------------"<<std::endl;
    #ifdef EIGEN_VECTORIZE
    std::cout<<" * use vectorization"<<std::endl;
    #else
    std::cout<<" * will not use vectorization"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSE
    std::cout<<" * use SSE"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSE2
    std::cout<<" * use SSE2"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSE3
    std::cout<<" * use SSE3"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSSE3
    std::cout<<" * use SSSE3"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSE4_1
    std::cout<<" * use SSE4.1"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_SSE4_2
    std::cout<<" * use SSE4.2"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_ALTIVEC
    std::cout<<" * use AltiVec"<<std::endl;
    #endif
    
    #ifdef EIGEN_VECTORIZE_NEON
    std::cout<<" * use Neon"<<std::endl;
    #endif 
}
