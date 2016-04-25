#include "gtest/gtest.h"

#include "Eigen/Eigen"

#include <ctime>

template<unsigned int N, class ETYPE, class PTYPE> void speedTestDynamic(const char* msg)
{
        
    ETYPE eigen_d(N);
    eigen_d.setZero();
    
    //benchmark how many iterations will give a run time of ~0.5s
    std::clock_t eigen_begin = std::clock();
    unsigned int toys = 0;
    while (double(std::clock()-eigen_begin)/CLOCKS_PER_SEC<1.0)
    {
        for (unsigned int i = 0; i < 10; ++i)
        {
            ETYPE eigen_a(N);
            ETYPE eigen_b(N);
            ETYPE eigen_c(N);
            for (unsigned int j = 0; j < N; ++j)
            {
                eigen_a[j]=i+j+2;
                eigen_b[j]=i-j+2;
                eigen_c[j]=0.2*i;
            }
            eigen_d += eigen_a+0.5*eigen_b+eigen_c/0.5;     
        }
        toys+=10;
    }
    
    //reset and start actual tests using estimates toys
    eigen_d.setZero();
    eigen_begin = std::clock();
    
    
    for (unsigned int i = 0; i<toys; ++i)
    {
        ETYPE eigen_a(N);
        ETYPE eigen_b(N);
        ETYPE eigen_c(N);
        for (unsigned int j = 0; j < N; ++j)
        {
            eigen_a[j]=i+j+2;
            eigen_b[j]=i-j+2;
            eigen_c[j]=0.2*i;
        }
        eigen_d += eigen_a+0.5*eigen_b-eigen_c/0.5;     
        
    }
    double eigen_secs = double(std::clock() - eigen_begin) / CLOCKS_PER_SEC;
    
    PTYPE eigen_sum = 0.0;
    for (unsigned int j = 0; j < N; ++j)
    {
        eigen_sum+=eigen_d[j];
    }

    PTYPE* plain_d = new PTYPE[N];
    for (unsigned int j = 0; j < N; ++j)
    {
        plain_d[j]=0;
    }
    
    std::clock_t plain_begin = std::clock();
    for (unsigned int i = 0; i<toys; ++i)
    {
        PTYPE* plain_a = new PTYPE[N];
        PTYPE* plain_b = new PTYPE[N];
        PTYPE* plain_c = new PTYPE[N];
        for (unsigned int j = 0; j < N; ++j)
        {
            plain_a[j]=i+j+2;
            plain_b[j]=i-j+2;
            plain_c[j]=0.2*i;

            plain_d[j] += plain_a[j]+0.5*plain_b[j]-plain_c[j]/0.5;     
        }
        delete[] plain_a;
        delete[] plain_b;
        delete[] plain_c;
    }
    double plain_secs = double(std::clock() - plain_begin) / CLOCKS_PER_SEC;
    
    PTYPE plain_sum = 0.0;
    for (unsigned int j = 0; j < N; ++j)
    {
        plain_sum+=plain_d[j];
    }
    
    if (typeid(PTYPE)==typeid(float))
    {
        EXPECT_FLOAT_EQ(eigen_sum,plain_sum);
    }
    else
    {
        EXPECT_EQ(eigen_sum,plain_sum);
    }
    printf("%10s: eigen(%6.3fs)/plain(%6.3fs)=%6.3f\n",msg,eigen_secs,plain_secs,eigen_secs/plain_secs);

}

TEST(Eigen, speed)
{
    using namespace Eigen;
    speedTestDynamic<1,VectorXf,float>("floatx1");
    speedTestDynamic<2,VectorXf,float>("floatx2");
    speedTestDynamic<4,VectorXf,float>("floatx4");
    speedTestDynamic<8,VectorXf,float>("floatx8");
    speedTestDynamic<16,VectorXf,float>("floatx16");
    speedTestDynamic<32,VectorXf,float>("floatx32");
    speedTestDynamic<64,VectorXf,float>("floatx64");
    speedTestDynamic<128,VectorXf,float>("floatx64");
    
    speedTestDynamic<1,VectorXd,double>("doublex1");
    speedTestDynamic<2,VectorXd,double>("doublex2");
    speedTestDynamic<4,VectorXd,double>("doublex4");
    speedTestDynamic<8,VectorXd,double>("doublex8");
    speedTestDynamic<16,VectorXd,double>("doublex16");
    speedTestDynamic<32,VectorXd,double>("doublex32");
    speedTestDynamic<64,VectorXd,double>("doublex64");
    speedTestDynamic<128,VectorXd,double>("doublex64");
}

