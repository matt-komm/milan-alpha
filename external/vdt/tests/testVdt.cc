#include "gtest/gtest.h"

#include "vdt/vdtMath.h"
#include <cmath>
#include <cstdio>
#include <functional>

template<class TYPE> double testFct(
    TYPE start, TYPE end, unsigned int N, 
    std::function<TYPE(TYPE)> fct, std::function<TYPE(TYPE)> refFct,
    const char* msg
    )
{
    unsigned int totalShifts = 0;
    unsigned int maxShifts = 0;
    unsigned int nTest = 0;
    TYPE value = start;
    while (value<end)
    {
        TYPE fast_result = fct(value);
        TYPE exact_result = refFct(value);
        unsigned int shifts = 0;
        TYPE iterate_result = fast_result;
        while (iterate_result!=exact_result and shifts<10000)
        {
            iterate_result = std::nextafter(iterate_result, exact_result);
            ++shifts;
        }
        totalShifts+=shifts;
        maxShifts = std::max(maxShifts,shifts);
        ++nTest;
        
        value += (end-start)/N;
    }
    printf("%s: avg. bit differences: %5.3f [max: %2i]\n",msg,1.0*totalShifts/nTest,maxShifts);
    EXPECT_LE(1.0*totalShifts/nTest,0.5);
    return 1.0*totalShifts/nTest;
}

TEST(vdt, sin)
{
    testFct<float>(-10.0,10.0,10000,
        [](float x) {return vdt::fast_sinf(x);},
        [](float x) {return std::sin(x);},
        "sin float"
    );
    testFct<double>(-10.0,10.0,10000,
        [](double x) {return vdt::fast_sin(x);},
        [](double x) {return std::sin(x);},
        "sin double"
    );
}

TEST(vdt, cos)
{
    testFct<float>(-10.0,10.0,10000,
        [](float x) {return vdt::fast_cosf(x);},
        [](float x) {return std::cos(x);},
        "cos float"
    );
    testFct<double>(-10.0,10.0,10000,
        [](double x) {return vdt::fast_cos(x);},
        [](double x) {return std::cos(x);},
        "cos double"
    );
}

TEST(vdt, tan)
{
    testFct<float>(-10.0,10.0,10000,
        [](float x) {return vdt::fast_tanf(x);},
        [](float x) {return std::tan(x);},
        "tan float"
    );
    testFct<double>(-10.0,10.0,10000,
        [](double x) {return vdt::fast_tan(x);},
        [](double x) {return std::tan(x);},
        "tan double"
    );
}

TEST(vdt, exp)
{
    testFct<float>(-10.0,10.0,10000,
        [](float x) {return vdt::fast_expf(x);},
        [](float x) {return std::exp(x);},
        "exp float"
    );
    testFct<double>(-10.0,10.0,10000,
        [](double x) {return vdt::fast_exp(x);},
        [](double x) {return std::exp(x);},
        "exp double"
    );
}

TEST(vdt, log)
{
    testFct<float>(0.001,1000.0,10000,
        [](float x) {return vdt::fast_logf(x);},
        [](float x) {return std::log(x);},
        "log float"
    );
    testFct<double>(0.001,1000.0,10000,
        [](double x) {return vdt::fast_log(x);},
        [](double x) {return std::log(x);},
        "log double"
    );
}

TEST(vdt, isqrt)
{
    testFct<double>(10.0,100.0,10000,
        [](double x) {return vdt::fast_isqrt(x);},
        [](double x) {return 1./std::sqrt(x);},
        "isqrt double"
    );
}


