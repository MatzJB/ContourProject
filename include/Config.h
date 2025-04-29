// config.h
// This header contains global variables used by Contours.
#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

#include "Contour.h"
#include "Line2.h"
#include "Point2.h"
#include "Vector2.h"
#include "Segment.h"
#include "Arc.h"

#define PI  3.14159265358979323846
inline double EPS = 1E-14; // should be large enough for double precision

constexpr auto RES = 100; // default resolution for arcs
constexpr int PRINT_PRECISION = 5; // precision for printing floats
