#pragma once
#include "Point2.h"

struct Vector2 /*!< Simple vector using REAL precision (from Point2) */
{
	Point2 p1;
	Point2 p2;
};

Vector2 operator+(const Point2& point1, const Point2& point2);
Vector2 operator-(const Point2& point1, const Point2& point2);
static bool operator==(const Point2& point1, const Point2& point2);

