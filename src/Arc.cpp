#include <Config.h>
#include <iostream>

Point2 Arc::getPoint(double t) const
{
	// Ensure t is within the valid range [0, 1]
	if (t < 0.0 || t > 1.0)
	{
		throw std::invalid_argument("t must be in the range [0, 1]");
	}

	// Calculate the angle based on the direction (forwards or backwards)
	double angle;
	if (forwards)
	{
		angle = start_angle + (end_angle - start_angle) * t; // Counterclockwise
	}
	else
	{
		angle = end_angle - (end_angle - start_angle) * t; // Clockwise
	}

	// Calculate the point on the arc
	double x = center.x + radius * std::cos(angle);
	double y = center.y + radius * std::sin(angle);
	return Point2({x, y});
}

Arc::Arc(const Point2& c, double r, double start, double end, unsigned res, bool fw)
{
	if (res < 2)
	{
		throw std::invalid_argument("resolution must be greater than 1");
	}

	if (r <= 0)
	{
		throw std::invalid_argument("radius must be positive and non zero");
	}
	if (fabs(end - start) > 2 * PI)
	{
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
Point2 Arc::getCoordinate(double t) const
{
	if (t < 0 || t > 1)
	{
		throw std::invalid_argument("argument is out of bounds");
	}

	if ((this)->forwards)
	{
		return getPoint(t);
	}
	return getPoint(1 - t);
}

bool Arc::operator==(const Segment& other) const
{
	auto arc = dynamic_cast<const Arc*>(&other);
	if (!arc) return false;
	return this->center.isCloseTo(arc->center, EPS) &&
		fabs(this->radius - arc->radius) < EPS &&
		fabs(this->start_angle - arc->start_angle) < EPS &&
		fabs(this->end_angle - arc->end_angle) < EPS &&
		this->forwards == arc->forwards;
}

// TODO: Maybe convert to string and flush prints after we are done?
void Arc::print(const std::string& padding) const
{
	std::cout << padding << "ARC\n";
	std::cout << "  " << padding << ((forwards) ? "counter " : "") << "clockwise\n";
	std::cout << "  " << padding << "center " << center.x << " " << center.y << "\n";
	std::cout << "  " << padding << "radius " << radius << "\n";
	std::cout << "  " << padding << "angle <-[" << start_angle << ", " << end_angle << "]\n";
}

std::vector<Point2> Arc::getLineStrip() const
{
	std::vector<Point2> points;

	for (unsigned int i = 0; i <= this->resolution; ++i)
	{
		double t = static_cast<double>(i) / this->resolution; // Map i to [0, 1]
		points.emplace_back(this->getCoordinate(t)); // Use getCoordinate to calculate the point
	}

	return points;
}
