#ifndef _SEGMENT_H
#define _SEGMENT_H
#endif

#include <Point2.h>
#include <Segment.h>
#include <stdexcept>
#include <Vector2.h>
#include <Line.h>

Line::Line(Point2 s, Point2 e)
	: start(s), end(e)
{
	if (fabs(start.x - end.x) < EPS && fabs(start.y - end.y) < EPS) {
		throw std::invalid_argument("Line start and end points are the same");
	}
}
// Gets coordinate on line, t<-[0,1]
Point2 Line::getCoordinate(REAL t) const
{
	Point2 p1;
	if ((this)->forwards)
	{
		p1.x = start.x + (end.x - start.x) * t;
		p1.y = start.y + (end.y - start.y) * t;
	}
	else
	{
		p1.x = end.x + (start.x - end.x) * t;
		p1.y = end.y + (start.y - end.y) * t;
	}
	return p1;
}

bool Line::operator==(const Segment& other) const {
	const Line* line = dynamic_cast<const Line*>(&other);
	if (!line) return false;

	return fabs((this)->start.x - line->start.x) < EPS &&
		fabs((this)->start.y - line->start.y) < EPS &&
		fabs((this)->end.x - line->end.x) < EPS &&
		fabs((this)->end.y - line->end.y) < EPS &&
		(this)->forwards == line->forwards;
}

void Line::print(const std::string& padding) const {
	std::cout << padding << "LINE\n";

	std::cout << "  "
		<< padding
		<< "(" << start.x << ", " << start.y << ") - "
		<< "(" << end.x << ", " << end.y << ") "
		<< (forwards ? "" : "(reverse)")
		<< "\n";
}

// Inspiration from OpenGL standard https://www.khronos.org/opengl/wiki/Primitive
std::vector<Point2> Line::getLineStrip(int resolution) const {
	return std::vector{ Point2({ start.x, start.y }), Point2({ end.x,end.y }) };
}

