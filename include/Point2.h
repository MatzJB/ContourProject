#pragma once

#include "Config.h"

struct Point2 {/*!< Simple 2-dimensional coordinate using REAL precision. */
	REAL x;
	REAL y;

	Point2 operator+(const Point2& point) const;
	Point2 operator-( const Point2& point) const;
	bool operator==( const Point2& point) const;
	[[nodiscard]] bool isTooFarTo(const Point2& point, REAL threshold) const;
};

