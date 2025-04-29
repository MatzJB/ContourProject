#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Line2.h"


// Test for getLineStrip with valid Line2
TEST(Line2Tests, GetLineStripValid)
{
	// Create a Line2 object
	Point2 start{0.0, 0.0};
	Point2 end{1.0, 1.0};
	Line2 line(start, end);

	// Get the line strip
	std::vector<Point2> line_strip = line.getLineStrip();

	// Verify the size of the line strip
	EXPECT_EQ(line_strip.size(), 2) << "Line strip should contain exactly two points.";

	// Verify the start point
	EXPECT_NEAR(line_strip[0].x, start.x, EPS) << "Start point X does not match.";
	EXPECT_NEAR(line_strip[0].y, start.y, EPS) << "Start point Y does not match.";

	// Verify the end point
	EXPECT_NEAR(line_strip[1].x, end.x, EPS) << "End point X does not match.";
	EXPECT_NEAR(line_strip[1].y, end.y, EPS) << "End point Y does not match.";
}

// Test for getLineStrip with reversed Line2
TEST(Line2Tests, GetLineStripReversed)
{
	// Create a Line2 object with reversed direction
	Point2 start{0.0, 0.0};
	Point2 end{1.0, 1.0};
	Line2 line(start, end, false);

	// Get the line strip
	std::vector<Point2> line_strip = line.getLineStrip();

	// Verify the size of the line strip
	EXPECT_EQ(line_strip.size(), 2) << "Line strip should contain exactly two points.";

	// Verify the start point (should still be the same as the original start)
	EXPECT_NEAR(line_strip[0].x, start.x, EPS) << "Start point X does not match.";
	EXPECT_NEAR(line_strip[0].y, start.y, EPS) << "Start point Y does not match.";

	// Verify the end point (should still be the same as the original end)
	EXPECT_NEAR(line_strip[1].x, end.x, EPS) << "End point X does not match.";
	EXPECT_NEAR(line_strip[1].y, end.y, EPS) << "End point Y does not match.";
}
