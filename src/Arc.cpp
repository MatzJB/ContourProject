#include "Arc.h"
#include "Segment.h"
#include "Contour.h"
#include "Point2.h"

Point2 Arc::getPoint(REAL t) const {
	REAL angle = Arc::start_angle + (end_angle - start_angle) * t;
	return Point2({ center.x + radius * cos(angle), center.y + radius * sin(angle) });
}

Arc::Arc(const Point2& c, REAL r, REAL start, REAL end, unsigned res, bool fw)
{
	if (resolution < 2)
	{
		throw std::invalid_argument("resolution must be greater than 1");
	}

	if (r <= 0) {
		throw std::invalid_argument("radius must be positive");
	}
	if (start > end) {
		throw std::invalid_argument("start angle must be less than end angle");
	}
	if (end - start > 2 * PI) {
		throw std::invalid_argument("arc is too large");
	}
	center = c;
	radius = r;
	start_angle = start;
	end_angle = end;
	forwards = fw;
	resolution = res;
}

// Gets coordinate on circle arc, t<-[0,1]
Point2 Arc::getCoordinate(REAL t) const
{
	if (t < 0 || t>1) {
		throw std::invalid_argument("argument is out of bounds");
	}

	if ((this)->Arc::forwards) // if clockwise we start from the end
	{
		return getPoint(1 - t);
	}
	else
	{
		return getPoint(t);
	}
}

bool Arc::operator==(const Segment& other) const {
	const Arc* arc = dynamic_cast<const Arc*>(&other);
	if (!arc) return false;
	return !this->center.isTooFarTo(arc->center, EPS) &&
		fabs(this->radius - arc->radius) < EPS &&
		fabs(this->start_angle - arc->start_angle) < EPS &&
		fabs(this->end_angle - arc->end_angle) < EPS &&
		this->forwards == arc->forwards;
}

// TODO: Maybe convert to string and flush prints after we are done?
void Arc::print(const std::string& padding) const {
	std::cout << padding << "ARC\n";
	std::cout << "  " << padding << ((forwards) ? "counter " : "") << "clockwise\n";
	std::cout << "  " << padding << "center " << center.x << " " << center.y << "\n";
	std::cout << "  " << padding << "radius " << radius << "\n";
	std::cout << "  " << padding << "angle <-[" << start_angle << ", " << end_angle << "]\n";
}

// Return a strip of 2D coordinates, so no repeated coordinates.
// maybe not use resolution, maybe put the resolution as part of the arc?
std::vector<Point2> Arc::getLineStrip()  const {
	std::vector<Point2> points;
	REAL angle;
	REAL step = std::fabs(end_angle - start_angle) / this->resolution;

	for (unsigned int i = 0; i <= this->resolution; ++i) {

		if (forwards)
		{
			angle = start_angle + i * step;
		}
		else
		{
			angle = end_angle - i * step;
		}
		REAL x = center.x + radius * std::cos(angle);
		REAL y = center.y + radius * std::sin(angle);
		points.emplace_back(Point2({ x, y }));
	}
	return points;
}

//Arc::Arc() = default;

