#include "gtest/gtest.h"  
#include "Contour.h"  
#include "Point2.h"  
#include "Arc.h"  
#include "Line2.h"  


// Test for valid Arc construction
TEST(ArcTests, ValidArcConstruction) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double startAngle = 0.0;
    double endAngle = 3.14; // PI
    unsigned int resolution = 20;
    bool forwards = true;

    // Expect no exceptions for valid input
    EXPECT_NO_THROW(Arc(center, radius, startAngle, endAngle, resolution, forwards));
}

// Test for invalid Arc construction: negative radius
TEST(ArcTests, NegativeRadiusThrowsError) {
    Point2 center{ 0, 0 };
    double negativeRadius = -5.0;
    double startAngle = 0.0;
    double endAngle = 3.14; // PI
    unsigned int resolution = 20;

    EXPECT_THROW(
        Arc(center, negativeRadius, startAngle, endAngle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when the radius is negative.";
}

// Test for invalid Arc construction: resolution less than 2
TEST(ArcTests, LowResolutionThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double startAngle = 0.0;
    double endAngle = PI*0.5; // PI
    unsigned int resolution = 1; // Invalid resolution

    EXPECT_THROW(
        Arc(center, radius, startAngle, endAngle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when resolution is less than 2.";
}

// Test for invalid Arc construction: arc too large
TEST(ArcTests, ArcTooLargeThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double startAngle = 0.0;
    double endAngle = 2 * 3.14 + 0.1; // Greater than 2 * PI
    unsigned int resolution = 20;

    EXPECT_THROW(
        Arc(center, radius, startAngle, endAngle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when the arc is too large.";
}

// Test for valid getCoordinate
TEST(ArcTests, GetCoordinateValid) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double startAngle = 0.0;
    double endAngle = PI;
    unsigned int resolution = 50;
    bool forwards = true;

    Arc arc(center, radius, startAngle, endAngle, resolution, forwards);

    // Test t = 0 (start of the arc)
    Point2 startPoint = arc.getCoordinate(0);
    EXPECT_NEAR(startPoint.x, center.x + radius, EPS);
    EXPECT_NEAR(startPoint.y, center.y, EPS);

    // Test t = 1 (end of the arc)
    Point2 endPoint = arc.getCoordinate(1);
    EXPECT_NEAR(endPoint.x, center.x-radius, EPS);
    EXPECT_NEAR(endPoint.y, center.y, EPS);
}

// Test for invalid getCoordinate: t out of bounds
TEST(ArcTests, GetCoordinateOutOfBoundsThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double startAngle = 0.0;
    double endAngle = 3.14; // PI
    unsigned int resolution = 20;
    bool forwards = true;

    Arc arc(center, radius, startAngle, endAngle, resolution, forwards);

    // Test t < 0
    EXPECT_THROW(arc.getCoordinate(-0.1), std::invalid_argument)
        << "getCoordinate should throw an exception when t is less than 0.";

    // Test t > 1
    EXPECT_THROW(arc.getCoordinate(1.1), std::invalid_argument)
        << "getCoordinate should throw an exception when t is greater than 1.";
}