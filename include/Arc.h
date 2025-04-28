#pragma once
#ifndef ARC_H
#define ARC_H
#endif

#include "Segment.h"
#include "Point2.h"
#include <vector>
#include <string>

class Arc : public Segment { /*!< Arc is a Segment consisting of a center Point2, radius and start and end angles.
	You can flip the direction by setting forwards */
public:
    Point2 center;
    double radius;
    double start_angle;
    double end_angle;
    bool forwards = true;
    unsigned int resolution;

    Arc(const Point2& c, double r, double start, double end, unsigned int resolution=20, bool fw = true);

    Point2 getCoordinate(double t) const override;
    bool operator==(const Segment& other) const override;
    void print(const std::string& padding) const override;
    std::vector<Point2> getLineStrip() const override;

private:
    Point2 getPoint(double t) const;
};
