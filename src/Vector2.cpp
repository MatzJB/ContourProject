#include <iostream>

#include "Config.h"
#include "Point2.h"
#include <Vector2.h>


Vector2 operator+(const Point2& point1, const Point2& point2)
{
	return { point1.x + point2.x, point1.y + point2.y };
}

Vector2 operator-(const Point2& point1, const Point2& point2)
{
	return { point1.x - point2.x, point1.y - point2.y };
}


static bool operator==(const Point2& point1, const Point2& point2)
{
	return fabs(point1.x - point2.x) < EPS && fabs(point1.y - point2.y) < EPS;
}

//static void printPoints(const Vector2::Vector2& v) {
//	for (const auto& point : v) {
//		std::cout << "(" << point.x << ", " << point.y << ")\n";
//	}
//}