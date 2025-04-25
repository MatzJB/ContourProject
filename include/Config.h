// config.h
#pragma once

// todo: check if I need to add variant and cmath here
#include <variant>
#include <cmath>
#include <cstdio>
#include <limits>
#include <iomanip>
#include <iostream>

// We do not know the precision of the input data, so we keep this open.
// EPS should be larger than the e_mach ofc, otherwise all tests will return nonsense.

// change so we can use anything, float double long double etc.
#ifdef USE_DOUBLE_PRECISION
using REAL = double;
#else
using REAL = double;
#endif
#define PI  3.14159265358979323846
#define EPS 0.000000000001


constexpr int print_precision = 5;

