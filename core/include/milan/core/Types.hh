#ifndef __MILAN_CORE_TYPES_H__
#define __MILAN_CORE_TYPES_H__

//#include "fadbad/fadiff.h"
#include "adept/adept.h"

#include <inttypes.h>
#include <cstddef>
#include <utility>

namespace milan
{

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef float float32;
typedef double float64;
typedef long double float128;

typedef std::size_t sizetype;

typedef adept::adouble Ftype;


static_assert(sizeof(int8)==1, "int8 is not of required size: 1");
static_assert(sizeof(uint8)==1, "uint8 is not of required size: 1");
static_assert(sizeof(int16)==2, "int16 is not of required size: 2");
static_assert(sizeof(uint16)==2, "uint16 is not of required size: 2");
static_assert(sizeof(int32)==4, "int32 is not of required size: 4");
static_assert(sizeof(uint32)==4, "uint32 is not of required size: 4");
static_assert(sizeof(int64)==8, "int64 is not of required size: 8");
static_assert(sizeof(uint64)==8, "uint64 is not of required size: 8");

static_assert(sizeof(float32)==4, "float32 is not of required size: 4");
static_assert(sizeof(float64)==8, "float64 is not of required size: 8");
static_assert(sizeof(float128)==16, "float128 is not of required size: 16");

}

#endif

