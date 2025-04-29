#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Line2.h"

// Test for contourFromPoints
TEST(ContourTests, ContourFromPoints) {
    // Test with valid input
    std::vector<Point2> points = {
        Point2{0, 0},
        Point2{1, 1},
        Point2{2, 2},
        Point2{3, 3}
    };

    Contour contour = contourFromPoints(points);

    // Verify the number of elements in the contour
    EXPECT_EQ(contour.getElements().size(), points.size() - 1)
        << "The number of elements in the contour should be one less than the number of points.";

    // Verify that the contour is valid
    EXPECT_TRUE(contour.isValid()) << "The contour created from the points should be valid.";

    // Verify that the contour's Line2 segments match the input points
    auto elements = contour.getElements();
    for (size_t i = 0; i < elements.size(); ++i) {
        const Line2& line = std::get<Line2>(elements[i]);

        // floating-point comparisons
        EXPECT_LT(fabs(line.getCoordinate(0).x - points[i].x), EPS)
            << "Line start point X does not match input point X.";
        EXPECT_LT(fabs(line.getCoordinate(0).y - points[i].y), EPS)
            << "Line start point Y does not match input point Y.";
        EXPECT_LT(fabs(line.getCoordinate(1).x - points[i + 1].x), EPS)
            << "Line end point X does not match input point X.";
        EXPECT_LT(fabs(line.getCoordinate(1).y - points[i + 1].y), EPS)
            << "Line end point Y does not match input point Y.";
    }

    // Test with invalid input (less than 2 points)
    std::vector<Point2> invalid_points = { Point2{0, 0} };
    EXPECT_THROW(contourFromPoints(invalid_points), std::invalid_argument)
        << "The function should throw an exception when fewer than two points are provided.";
}