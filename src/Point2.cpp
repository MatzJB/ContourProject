#include "Config.h"

/*
 * As this is a toy example and to make things simple, I use a struct because it is
 *  faster because it is allocated on the Stack, as opposed to the Heap.
 *  If we want to use this in a double world example, we might want to consider
 *  vectorizing the code (SSE), by storing the coordinates in separate vectors.
 */

Point2 Point2::operator+(const Point2& point) const
{
	return { x + point.x, y + point.y };
}

Point2 Point2::operator-(const Point2& point) const
{
	return { x - point.x, y - point.y };

}

bool Point2::operator==(const Point2& point) const
{
	return fabs(x - point.x) < EPS && fabs(y - point.y) < EPS;
}

// Fast check of distance between points, purposely avoiding using sqrt
bool Point2::isCloseTo(const Point2& point, double threshold) const
{
	return (x - point.x) * (x - point.x) +
		(y - point.y) * (y - point.y) < threshold * threshold;
}

