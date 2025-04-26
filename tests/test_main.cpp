#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Arc.h"
#include "Line.h"
#include <thread>


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

    // Add invalid lines
    Line l1(Point2({ 0, 0 }), Point2({ 1, 1 }));
    Line l2(Point2({ 1, 1 }), Point2({ 2, 0 }));
    Line l3(Point2({ 2, 0 }), Point2({ 3, 3 }));
    Line l4(Point2({ 3, 3 }), Point2({ 4, 0 }));
    Line l5(Point2({ 4, 1.001 * EPS }), Point2({ 5, 5 }));

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

    // Add invalid lines to contour2
    Line l1(Point2({ 0, 0 }), Point2({ 1, 1 }));
    Line l2(Point2({ 1, 1 }), Point2({ 2, 0 }));
    contour2.addItem(l1);
    contour2.addItem(l2);

    // Merge contours
    std::vector<Contour> joined = { contour1, contour2 };

    // Check uniqueness
    EXPECT_TRUE(vectorContoursUniqueness(joined));
}