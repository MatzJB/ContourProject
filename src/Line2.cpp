
#include <Point2.h>
#include <Segment.h>
#include <Line2.h>

Line2::Line2(Point2 s, Point2 e, bool fw)
{
	start = s;
	end = e;
	forwards = fw;

	if (fabs(start.x - end.x) < EPS && fabs(start.y - end.y) < EPS) {
		throw std::invalid_argument("Line2 start and end points are the same");
	}
}
// Gets coordinate on Line2, t<-[0,1]
Point2 Line2::getCoordinate(double t) const
{
	Point2 p1;
	/*
	 * Please note: According to IEEE 754, precision is not an issue here, if t=0 or 1, we are at start or end.
	  The "problem" occur when we use fractions, for instance 0.333333... cannot be represented exactly in binary form.
	 */
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

bool Line2::operator==(const Segment& other) const {
	const Line2* line = dynamic_cast<const Line2*>(&other);
	if (!line) return false;

	return fabs((this)->start.x - line->start.x) < EPS &&
		fabs((this)->start.y - line->start.y) < EPS &&
		fabs((this)->end.x - line->end.x) < EPS &&
		fabs((this)->end.y - line->end.y) < EPS &&
		(this)->forwards == line->forwards;
}

void Line2::print(const std::string& padding) const {
	std::cout << padding << "Line2\n";

	std::cout << "  "
		<< padding
		<< "(" << start.x << ", " << start.y << ") - "
		<< "(" << end.x << ", " << end.y << ") "
		<< (forwards ? "" : "(reverse)")
		<< "\n";
}

// Inspiration from OpenGL standard https://www.khronos.org/opengl/wiki/Primitive
std::vector<Point2> Line2::getLineStrip() const {
	return std::vector{ Point2({ start.x, start.y }), Point2({ end.x,end.y }) };
}
