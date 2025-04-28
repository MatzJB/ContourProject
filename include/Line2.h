#pragma once
#ifndef Line2_H
#define Line2_H

#include <vector>
#include <string>
#include "Segment.h"
#include "Point2.h"

class Line2 : public Segment { /*!< Line2 is a Segment consisting of two Point2, you can flip the direction by setting forwards */
    Point2 start;
    Point2 end;
    bool forwards = true;

public:
    Line2::Line2(Point2 s, Point2 e, bool fw = true);

    Point2 getCoordinate(double t) const override;

    bool operator==(const Segment& other) const override;
    
    void print(const std::string& padding) const override;

    std::vector<Point2> getLineStrip() const override;
};

#endif  
