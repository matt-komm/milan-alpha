#include "Eigen/Eigen"

#include <iostream>
    

int main()
{
    std::cout<<"Summary of Eigen config"<<std::endl;
    std::cout<<"-----------------------"<<std::endl;
    if (EIGEN_COMP_GNUC)
    {
        std::cout<<" * compiler is gcc"<<std::endl;
    }
    if (EIGEN_COMP_CLANG)
    {
        std::cout<<" * compiler is clang"<<std::endl;
    }
    if (EIGEN_COMP_LLVM)
    {
        std::cout<<" * compiler backend is llvm"<<std::endl;
    }
    if (EIGEN_COMP_ICC)
    {
        std::cout<<" * compiler is icc"<<std::endl;
    }
    if (EIGEN_OS_UNIX)
    {
        std::cout<<" * os is unix variant"<<std::endl;
    }
    if (EIGEN_OS_LINUX)
    {
        std::cout<<" * os is linux based"<<std::endl;
    }
    if (EIGEN_OS_MAC)
    {
        std::cout<<" * os is OSX"<<std::endl;
    }
    if (EIGEN_ALIGN)
    {
        std::cout<<" * use aligned arrays"<<std::endl;
    }
    
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
