#ifndef _SCALAR_H
#define _SCALAR_H


#define _USE_MATH_DEFINES
#include <math.h>

//#include <stdlib.h>//size_t for MSVC 6.0
#include <float.h>


//The Real type abstracts floating point numbers, to easily switch between double and single floating point precision.
#if defined(USE_DOUBLE_PRECISION)
	typedef double Real;
#else
	typedef float Real;
#endif


#ifdef BT_USE_SSE
	typedef __m128 SimdFloat4;
#endif


#endif //_SCALAR_H
