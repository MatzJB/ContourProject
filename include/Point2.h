#pragma once

#include <vector>

#include "Config.h"

struct Point2 {/*!< Simple 2-dimensional coordinate using double precision. */
	double x;
	double y;

	Point2 operator+(const Point2& point) const;
	Point2 operator-( const Point2& point) const;
	bool operator==( const Point2& point) const;
	[[nodiscard]] bool isCloseTo(const Point2& point, double threshold) const;
	[[nodiscard]] Point2 getCoordinate(double t) const;
};

