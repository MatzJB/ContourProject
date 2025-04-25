#pragma once
#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>

// Forward declarations or includes for required types
#include "Segment.h"
#include "Point2.h"

class Line : public Segment { /*!< Line is a Segment consisting of two Point2, you can flip the direction by setting forwards */
    Point2 start;
    Point2 end;
    bool forwards = true;

public:
    Line(Point2 s, Point2 e);

    Point2 getCoordinate(REAL t) const override;

    bool operator==(const Segment& other) const override;
    
    void print(const std::string& padding) const override;

    std::vector<Point2> getLineStrip() const override;
};

#endif  
