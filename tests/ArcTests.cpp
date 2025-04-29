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
    double negative_radius = -5.0;
    double start_angle = 0.0;
    double end_angle = 3.14; // PI
    unsigned int resolution = 20;

    EXPECT_THROW(
        Arc(center, negative_radius, start_angle, end_angle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when the radius is negative.";
}

// Test for invalid Arc construction: resolution less than 2
TEST(ArcTests, LowResolutionThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double start_angle = 0.0;
    double end_angle = PI*0.5; // PI
    unsigned int resolution = 1; // Invalid resolution

    EXPECT_THROW(
        Arc(center, radius, start_angle, end_angle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when resolution is less than 2.";
}

// Test for invalid Arc construction: arc too large
TEST(ArcTests, ArcTooLargeThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double start_angle = 0.0;
    double end_angle = 2 * 3.14 + 0.1; // Greater than 2 * PI
    unsigned int resolution = 20;

    EXPECT_THROW(
        Arc(center, radius, start_angle, end_angle, resolution, true),
        std::invalid_argument
    ) << "Arc constructor should throw an exception when the arc is too large.";
}

// Test for valid getCoordinate
TEST(ArcTests, GetCoordinateValid) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double start_angle = 0.0;
    double end_angle = PI;
    unsigned int resolution = 50;
    bool forwards = true;

    Arc arc(center, radius, start_angle, end_angle, resolution, forwards);

    // Test t = 0 (start of the arc)
    Point2 start_point = arc.getCoordinate(0);
    EXPECT_NEAR(start_point.x, center.x + radius, EPS);
    EXPECT_NEAR(start_point.y, center.y, EPS);

    // Test t = 1 (end of the arc)
    Point2 end_point = arc.getCoordinate(1);
    EXPECT_NEAR(end_point.x, center.x-radius, EPS);
    EXPECT_NEAR(end_point.y, center.y, EPS);
}

// Test for invalid getCoordinate: t out of bounds
TEST(ArcTests, GetCoordinateOutOfBoundsThrowsError) {
    Point2 center{ 0, 0 };
    double radius = 5.0;
    double start_angle = 0.0;
    double end_angle = 3.14; // PI
    unsigned int resolution = 20;
    bool forwards = true;

    Arc arc(center, radius, start_angle, end_angle, resolution, forwards);

    // Test t < 0
    EXPECT_THROW(arc.getCoordinate(-0.1), std::invalid_argument)
        << "getCoordinate should throw an exception when t is less than 0.";

    // Test t > 1
    EXPECT_THROW(arc.getCoordinate(1.1), std::invalid_argument)
        << "getCoordinate should throw an exception when t is greater than 1.";
}