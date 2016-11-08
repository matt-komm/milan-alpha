#ifndef __MILAN_TEST_MACROS_H__
#define __MILAN_TEST_MACROS_H__

#include "gtest/gtest.h"

#include <limits>
#include <cmath>

namespace milan
{

template<class T> inline bool approximatelyEqual(const T& a, const T& b, const T& epsilon=std::numeric_limits<T>::epsilon())
{
    return std::fabs(a - b) <= ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
}

template<class T> inline bool essentiallyEqual(const T& a, const T& b, const T& epsilon=std::numeric_limits<T>::epsilon())
{
    return std::fabs(a - b) <= ( (std::fabs(a) > std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
}

template<class T> inline bool definitelyGreaterThan(const T& a, const T& b, const T& epsilon=std::numeric_limits<T>::epsilon())
{
    return (a - b) > ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
}

template<class T> inline bool definitelyLessThan(const T& a, const T& b, const T& epsilon=std::numeric_limits<T>::epsilon())
{
    return (b - a) > ( (std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) * epsilon);
}

}

#endif

#define EXPECT_APPROX_EQ(a,b) \
    EXPECT_EQ(milan::approximatelyEqual(a,b),true) <<"expected "<<#a<<" which is "<<a<<" to be equal to "<<#b<<" which is "<<b;

#define EXPECT_APPROX_FLOAT_EQ(a,b) \
    EXPECT_EQ(milan::approximatelyEqual<float>(a,b),true) <<"expected "<<#a<<" which is "<<a<<" to be equal to "<<#b<<" which is "<<b;

