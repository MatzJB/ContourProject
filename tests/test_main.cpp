#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Arc.h"
#include "Line2.h"
#include <thread>


TEST(ContourEdgeCases, EmptyContour) {
    Contour contour;

    // An empty contour should be valid
    EXPECT_TRUE(contour.isValid());

    // The number of elements should be zero
    EXPECT_EQ(contour.getElements().size(), 0);
}

TEST(ContourEdgeCases, SingleElementContour) {
    Contour contour;

    // Add a single Line2
    Line2 Line2(Point2({ 0, 0 }), Point2({ 1, 1 }));
    contour.addItem(Line2);

    // A single-element contour should be valid
    EXPECT_TRUE(contour.isValid());

    // The number of elements should be one
    EXPECT_EQ(contour.getElements().size(), 1);
}

TEST(ContourThreadedValidationTest, FilterValidAndInvalidContoursCorrectly)
{
    // Setup
    Contour contour1; // valid contour
    Contour contour2; // invalid contour
    Contour contour3; // invalid contour
    Contour contour4; // valid contour


    // Create "Periodic half circles" - valid
    contour1.addItem(Arc(Point2({ 0, 0 }), 1, 0, PI, RES, true));
    contour1.addItem(Arc(Point2({ 2, 0 }), 1, PI, 2 * PI, RES));
    contour1.addItem(Arc(Point2({ 4, 0 }), 1, 0, PI, RES, true));
    contour1.addItem(Arc(Point2({ 6, 0 }), 1, PI, 2 * PI, RES));

    // Create "Line2s" - invalid
    contour2.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
    contour2.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 0 })));
    contour2.addItem(Line2(Point2({ 2, 0 }), Point2({ 3, 3 })));
    contour2.addItem(Line2(Point2({ 3, 3 }), Point2({ 4, 0 })));
    contour2.addItem(Line2(Point2({ 4, 1.001 * EPS }), Point2({ 5, 5 })));

    // Create "Capsule" - invalid
    contour3.addItem(Arc(Point2({ 0, 0 }), 1, 0, PI));
    contour3.addItem(Line2(Point2({ -1, 0 }), Point2({ -1, -3 })));
    contour3.addItem(Arc(Point2({ 0, -3 }), 1, PI, 2 * PI));
    contour3.addItem(Line2(Point2({ 1 + EPS, -3 }), Point2({ 1, 0 })));

    // Create "Rectangle" - valid
    contour4.addItem(Line2(Point2({ 0, 0 }), Point2({ 10, 0 })));
    contour4.addItem(Line2(Point2({ 10, 0 }), Point2({ 10, 10 + 0.9 * EPS })));
    contour4.addItem(Line2(Point2({ 10, 10 }), Point2({ 0, 10 })));
    contour4.addItem(Line2(Point2({ 0, 10 }), Point2({ 0, 0 })));

    std::vector<Contour> contours = { contour1, contour2, contour3, contour4 };

    // Expected counts
    const int valid_expected = 2;
    const int invalid_expected = 2;

    std::vector<Contour> validContours;
    std::vector<Contour> invalidContours;

    // Launch threads
    std::thread thread_valid(filterValidStateContour, std::cref(contours), std::ref(validContours), true);
    std::thread thread_invalid(filterValidStateContour, std::cref(contours), std::ref(invalidContours), false);

    thread_valid.join();
    thread_invalid.join();

    // Validation
    int valid_result = 0;
    for (const auto& c : validContours) {
        if (c.isValid()) {
            valid_result++;
        }
    }
    EXPECT_EQ(valid_result, valid_expected) << "Valid contours were not filtered correctly.";

    int invalid_result = 0;
    for (const auto& c : invalidContours) {
        if (!c.isValid()) {
            invalid_result++;
        }
    }
    EXPECT_EQ(invalid_result, invalid_expected) << "Invalid contours were not filtered correctly.";

    // Check uniqueness
    std::vector<Contour> joined = validContours;
    joined.insert(joined.end(), invalidContours.begin(), invalidContours.end());

    EXPECT_TRUE(vectorContoursUniqueness(joined)) << "Contours are not unique after joining.";
}

// Test for valid contours
TEST(ContourTests, ValidContours) {
    Contour contour1;

    // Add valid arcs
    Arc a1(Point2({ 0, 0 }), 1, 0, PI, 20, true);
    Arc a2(Point2({ 2, 0 }), 1, PI, 2 * PI, 20);
    Arc a3(Point2({ 4, 0 }), 1, 0, PI, 20, true);
    Arc a4(Point2({ 6, 0 }), 1, PI, 2 * PI, 20);

    contour1.addItem(a1);
    contour1.addItem(a2);
    contour1.addItem(a3);
    contour1.addItem(a4);

    EXPECT_TRUE(contour1.isValid());
}

// Test for invalid contours
TEST(ContourTests, InvalidContours) {
    Contour contour2;

    // Add invalid Line2s
    Line2 l1(Point2({ 0, 0 }), Point2({ 1, 1 }));
    Line2 l2(Point2({ 1, 1 }), Point2({ 2, 0 }));
    Line2 l3(Point2({ 2, 0 }), Point2({ 3, 3 }));
    Line2 l4(Point2({ 3, 3 }), Point2({ 4, 0 }));
    Line2 l5(Point2({ 4, 1.001 * EPS }), Point2({ 5, 5 }));

    contour2.addItem(l1);
    contour2.addItem(l2);
    contour2.addItem(l3);
    contour2.addItem(l4);
    contour2.addItem(l5);

    EXPECT_FALSE(contour2.isValid());
}

// Test for uniqueness in joined contours
TEST(ContourTests, UniqueContours) {
    Contour contour1, contour2;

    // Add valid arcs to contour1
    Arc a1(Point2({ 0, 0 }), 1, 0, PI, 20, true);
    Arc a2(Point2({ 2, 0 }), 1, PI, 2 * PI, 20);
    contour1.addItem(a1);
    contour1.addItem(a2);

    // Add invalid Line2s to contour2
    Line2 l1(Point2({ 0, 0 }), Point2({ 1, 1 }));
    Line2 l2(Point2({ 1, 1 }), Point2({ 2, 0 }));
    contour2.addItem(l1);
    contour2.addItem(l2);

    // Merge contours
    std::vector<Contour> joined = { contour1, contour2 };

    // Check uniqueness
    EXPECT_TRUE(vectorContoursUniqueness(joined));
}

TEST(ContourEdgeCases, AddItemToCenter) {
    Contour contour;

    // Add initial elements
    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));
    // fix tests here
    // Add an element to the center
    Line2 newLine2(Point2({ 0.5, 0.5 }), Point2({ 1.5, 1.5 }));
    contour.addItemToCenter((ContourElement) newLine2);


    // Check the number of elements
    EXPECT_EQ(contour.getElements().size(), 3);

    // Check that the new element is in the center
    auto elements = contour.getElements();
    Line2 centerLine2 = std::get<Line2>(elements[1]);
    

	EXPECT_EQ(newLine2, centerLine2);
}

TEST(ContourEdgeCases, ClearContour) {
    Contour contour;

    // Add some elements
    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));

    // Clear the contour
    contour.clear();

    // The contour should now be empty
    EXPECT_EQ(contour.getElements().size(), 0);

    // The contour should still be valid
    EXPECT_TRUE(contour.isValid());
}


TEST(ContourEdgeCases, ClearAtInvalidIndex) {
    Contour contour;

    // Add some elements
    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));

    // Attempt to clear an invalid index
    EXPECT_THROW(contour.clearAtIndex(5), std::out_of_range);
    EXPECT_THROW(contour.clearAtIndex(-1), std::out_of_range);
}
//
//
//TEST(ContourEdgeCases, OverlappingElements) {
//    Contour contour;
//
//    // Add overlapping elements
//    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
//    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));
//    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 3, 3 }))); // Overlaps with the second Line2
//
//    // The contour should still be valid
//    EXPECT_TRUE(contour.isValid());
//}
//
//TEST(ContourEdgeCases, MixedValidity) {
//    Contour contour;
//
//    // Add valid elements
//    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
//    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));
//
//    // Add an invalid element
//    contour.addItem(Line2(Point2({ 10, 10 }), Point2({ 11, 11 }))); // Too far from the previous Line2
//
//    // The contour should be invalid
//    EXPECT_FALSE(contour.isValid());
//}
//
//
//TEST(ContourEdgeCases, ArcWithZeroResolution) {
//    Contour contour;
//
//    // Add an arc with zero resolution
//    Arc arc(Point2({ 0, 0 }), 1, 0, PI, 0);
//    contour.addItem(arc);
//
//    // The contour should still be valid
//    EXPECT_TRUE(contour.isValid());
//}
//
//
//TEST(ContourEdgeCases, DuplicateElements) {
//    Contour contour;
//
//    // Add duplicate elements
//    Line2 Line2(Point2({ 0, 0 }), Point2({ 1, 1 }));
//    contour.addItem(Line2);
//    contour.addItem(Line2);
//
//    // The contour should still be valid
//    EXPECT_TRUE(contour.isValid());
//
//    // The number of elements should be two
//    EXPECT_EQ(contour.getElements().size(), 2);
//}

//
//// Test for creating a contour from a set of points
//TEST(ContourEdgeCases, ContourFromCoordinates) {
//    // Define a set of points
//    std::vector<Point2> points;
//    points.emplace_back(Point2({ 0, 0 }));
//    points.emplace_back(Point2({ 1, 1 }));
//    points.emplace_back(Point2({ 5, 6 }));
//    points.emplace_back(Point2({ 10, 10 }));
//    points.emplace_back(Point2({ 15, 15 }));
//    points.emplace_back(Point2({ 15, 25 }));
//
//    // Create a contour from the points
//    Contour contour = contourFromPoints(points);
//
//    // Verify the number of elements in the contour
//    EXPECT_EQ(contour.getElements().size(), points.size() - 1)
//        << "The number of elements in the contour should be one less than the number of points.";
//
//    // Verify that the contour is valid
//    EXPECT_TRUE(contour.isValid()) << "The contour created from the points should be valid.";
//
//    // Verify that the contour's Line2 segments match the input points
//    auto elements = contour.getElements();
//    for (size_t i = 0; i < elements.size(); ++i) {
//        const Line2& line = std::get<Line2>(elements[i]);
//
//
//    Point2 start = line.getCoordinate(0);
//    Point2 end = line.getCoordinate(1);
//    Point2 startPoints = points[i];
//    Point2 endPoints = points[i+1];
//
//
//        EXPECT_EQ(start.x, points[i].x) << "Line start point does not match input point.";
//        EXPECT_EQ(start.y, points[i].y) << "Line start point does not match input point.";
//        EXPECT_EQ(end.x, points[i + 1].x) << "Line end point does not match input point.";
//        EXPECT_EQ(end.y, points[i + 1].y) << "Line end point does not match input point.";
//        
//    }
//}
//
//int main(int argc, char** argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}