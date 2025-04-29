#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Line2.h"


// Test for copy constructor
TEST(ContourTests, CopyConstructor) {
    // Create a contour and add elements
    Contour original;
    original.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    original.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));

    // Copy the contour
    Contour copy = original;

    // Verify the copied contour has the same elements
    EXPECT_EQ(copy.getElements().size(), original.getElements().size())
        << "Copied contour should have the same number of elements as the original.";

    auto originalElements = original.getElements();
    auto copyElements = copy.getElements();

    for (size_t i = 0; i < originalElements.size(); ++i) {
        const Line2& originalLine = std::get<Line2>(originalElements[i]);
        const Line2& copyLine = std::get<Line2>(copyElements[i]);

        EXPECT_EQ(originalLine.getCoordinate(0).x, copyLine.getCoordinate(0).x)
            << "Copied contour start point X does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(0).y, copyLine.getCoordinate(0).y)
            << "Copied contour start point Y does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(1).x, copyLine.getCoordinate(1).x)
            << "Copied contour end point X does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(1).y, copyLine.getCoordinate(1).y)
            << "Copied contour end point Y does not match the original.";
    }

    // Verify the original contour is unaffected
    EXPECT_TRUE(original.isValid()) << "Original contour should remain valid after copying.";
}

// Test for move constructor
TEST(ContourTests, MoveConstructor) {
    // Create a contour and add elements
    Contour original;
    original.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    original.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));

    // Move the contour
    Contour moved = std::move(original);

    // Verify the moved contour has the elements
    EXPECT_EQ(moved.getElements().size(), 2)
        << "Moved contour should have the same number of elements as the original.";

    auto movedElements = moved.getElements();
    const Line2& movedLine = std::get<Line2>(movedElements[0]);
    EXPECT_EQ(movedLine.getCoordinate(0).x, 0);
    EXPECT_EQ(movedLine.getCoordinate(0).y, 0);
    EXPECT_EQ(movedLine.getCoordinate(1).x, 1);
    EXPECT_EQ(movedLine.getCoordinate(1).y, 1);

    // Verify the original contour is in a valid but empty state
    EXPECT_EQ(original.getElements().size(), 0)
        << "Original contour should be empty after being moved.";
    EXPECT_TRUE(original.isValid()) << "Original contour should remain valid after being moved.";
}

// Test for copy assignment operator
TEST(ContourTests, CopyAssignment) {
    // Create a contour and add elements
    Contour original;
    original.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    original.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));

    // Assign the contour to another
    Contour assigned;
    assigned = original;

    // Verify the assigned contour has the same elements
    EXPECT_EQ(assigned.getElements().size(), original.getElements().size())
        << "Assigned contour should have the same number of elements as the original.";

    auto originalElements = original.getElements();
    auto assignedElements = assigned.getElements();
    for (size_t i = 0; i < originalElements.size(); ++i) {
        const Line2& originalLine = std::get<Line2>(originalElements[i]);
        const Line2& assignedLine = std::get<Line2>(assignedElements[i]);

        EXPECT_EQ(originalLine.getCoordinate(0).x, assignedLine.getCoordinate(0).x)
            << "Assigned contour start point X does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(0).y, assignedLine.getCoordinate(0).y)
            << "Assigned contour start point Y does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(1).x, assignedLine.getCoordinate(1).x)
            << "Assigned contour end point X does not match the original.";
        EXPECT_EQ(originalLine.getCoordinate(1).y, assignedLine.getCoordinate(1).y)
            << "Assigned contour end point Y does not match the original.";
    }

    // Verify the original contour is unaffected
    EXPECT_TRUE(original.isValid()) << "Original contour should remain valid after assignment.";
}

// Test for move assignment operator
TEST(ContourTests, MoveAssignment) {
    // Create a contour and add elements
    Contour original;
    original.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    original.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));

    // Assign the contour to another using move
    Contour assigned;
    assigned = std::move(original);

    // Verify the assigned contour has the elements
    EXPECT_EQ(assigned.getElements().size(), 2)
        << "Assigned contour should have the same number of elements as the original.";

    auto assignedElements = assigned.getElements();
    const Line2& assignedLine = std::get<Line2>(assignedElements[0]);
    EXPECT_EQ(assignedLine.getCoordinate(0).x, 0);
    EXPECT_EQ(assignedLine.getCoordinate(0).y, 0);
    EXPECT_EQ(assignedLine.getCoordinate(1).x, 1);
    EXPECT_EQ(assignedLine.getCoordinate(1).y, 1);

    // Verify the original contour is in a valid but empty state
    EXPECT_EQ(original.getElements().size(), 0)
        << "Original contour should be empty after being moved.";
    EXPECT_TRUE(original.isValid()) << "Original contour should remain valid after being moved.";
}

// Test for valid contours
TEST(ContourTests, ValidContours) {
    Contour contour;

    // Add valid arcs
    int resolution = 200;
    Arc a1(Point2({ 0, 0 }), 1, 0, PI, resolution);
    Arc a2(Point2({ -2, 0 }), 1, 2*PI,  PI, resolution, false);
    Arc a3(Point2({ -4, 0 }), 1, 0, PI, resolution);
    Arc a4(Point2({ -6, 0 }), 1, 0,  PI, resolution, false);

    contour.addItem(a1);
    contour.addItem(a2);
    contour.addItem(a3);
    contour.addItem(a4);


    EXPECT_TRUE(contour.isValid());


    EXPECT_EQ(contour.getElements().size(), 4);
    //contour.exportContourToSVG("c:\\temp\\valid_contour.svg", 1.0);

}
#include "gtest/gtest.h"
#include "Contour.h"  // Or whatever your contour header is called

// Test for invalid contours
TEST(ContourTests, InvalidContours) {
    Contour contour2;

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

    Arc a1(Point2({ 0, 0 }), 1, 0, PI, 20, true);
    Arc a2(Point2({ 2, 0 }), 1, PI, 2 * PI, 20);
    contour1.addItem(a1);
    contour1.addItem(a2);

    Line2 l1(Point2({ 0, 0 }), Point2({ 1, 1 }));
    Line2 l2(Point2({ 1, 1 }), Point2({ 2, 0 }));
    contour2.addItem(l1);
    contour2.addItem(l2);

    std::vector<Contour> joined = { contour1, contour2 };

    EXPECT_TRUE(vectorContoursUniqueness(joined));
}
