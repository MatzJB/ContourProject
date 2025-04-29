#pragma once
#include <vector>
#include "Point2.h"

// TODO: maybe add matrix for rotation and pivot point rot scaling and rotation
class Segment {
	/*!< Segment is a base class for Line2 and Arc. It defines the interface for getting coordinates, printing and comparing segments */
public:
	virtual ~Segment() = default;
	virtual Point2 getCoordinate(double t) const = 0;
	virtual void print(const std::string& padding) const = 0;
	virtual bool operator==(const Segment& other) const = 0;
	virtual std::vector<Point2> getLineStrip() const = 0;
};