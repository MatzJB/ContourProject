#include <filesystem>

#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Line2.h"

#include "Arc.h"
#include <fstream>

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

    auto original_elements = original.getElements();
    auto copy_elements = copy.getElements();

    for (size_t i = 0; i < original_elements.size(); ++i) {
        const Line2& original_line = std::get<Line2>(original_elements[i]);
        const Line2& copy_line = std::get<Line2>(copy_elements[i]);

        EXPECT_EQ(original_line.getCoordinate(0).x, copy_line.getCoordinate(0).x)
            << "Copied contour start point X does not match the original.";
        EXPECT_EQ(original_line.getCoordinate(0).y, copy_line.getCoordinate(0).y)
            << "Copied contour start point Y does not match the original.";
        EXPECT_EQ(original_line.getCoordinate(1).x, copy_line.getCoordinate(1).x)
            << "Copied contour end point X does not match the original.";
        EXPECT_EQ(original_line.getCoordinate(1).y, copy_line.getCoordinate(1).y)
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
    const Line2& moved_line = std::get<Line2>(movedElements[0]);
    EXPECT_EQ(moved_line.getCoordinate(0).x, 0);
    EXPECT_EQ(moved_line.getCoordinate(0).y, 0);
    EXPECT_EQ(moved_line.getCoordinate(1).x, 1);
    EXPECT_EQ(moved_line.getCoordinate(1).y, 1);

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

    auto original_elements = original.getElements();
    auto assigned_Elements = assigned.getElements();
    for (size_t i = 0; i < original_elements.size(); ++i) {
        const Line2& original_line = std::get<Line2>(original_elements[i]);
        const Line2& assigned_line = std::get<Line2>(assigned_Elements[i]);

        EXPECT_EQ(original_line.getCoordinate(0).x, assigned_line.getCoordinate(0).x)
            << "Assigned contour start point X does not match the original.";
        EXPECT_EQ(original_line.getCoordinate(0).y, assigned_line.getCoordinate(0).y)
            << "Assigned contour start point Y does not assigned_line the original.";
        EXPECT_EQ(original_line.getCoordinate(1).x, assigned_line.getCoordinate(1).x)
            << "Assigned contour end point X does not assigned_lineassigned_line the original.";
        EXPECT_EQ(original_line.getCoordinate(1).y, assigned_line.getCoordinate(1).y)
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
    const Line2& assigned_line = std::get<Line2>(assignedElements[0]);
    EXPECT_EQ(assigned_line.getCoordinate(0).x, 0);
    EXPECT_EQ(assigned_line.getCoordinate(0).y, 0);
    EXPECT_EQ(assigned_line.getCoordinate(1).x, 1);
    EXPECT_EQ(assigned_line.getCoordinate(1).y, 1);

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
}

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


// Test for an empty contour
TEST(ContourTests, EmptyContour) {
    Contour empty_contour;

    // Verify the contour is valid but has no elements
    EXPECT_TRUE(empty_contour.isValid()) << "Empty contour should be valid.";
    EXPECT_EQ(empty_contour.getElements().size(), 0) << "Empty contour should have no elements.";
}

// Test for a contour with a single segment
TEST(ContourTests, SingleSegmentContour) {
    Contour single_SegmentContour;

    // Add a single line segment
    Line2 line(Point2{ 0, 0 }, Point2{ 1, 1 });
    single_SegmentContour.addItem(line);

    // Verify the contour is valid and has one element
    EXPECT_TRUE(single_SegmentContour.isValid()) << "Single-segment contour should be valid.";
    EXPECT_EQ(single_SegmentContour.getElements().size(), 1) << "Single-segment contour should have one element.";

    // Verify the segment is correctly stored
    auto elements = single_SegmentContour.getElements();
    const Line2& stored_line = std::get<Line2>(elements[0]);
    EXPECT_EQ(stored_line.getCoordinate(0).x, 0) << "Start point X of the line does not match.";
    EXPECT_EQ(stored_line.getCoordinate(0).y, 0) << "Start point Y of the line does not match.";
    EXPECT_EQ(stored_line.getCoordinate(1).x, 1) << "End point X of the line does not match.";
    EXPECT_EQ(stored_line.getCoordinate(1).y, 1) << "End point Y of the line does not match.";
}

// Test for adding an item at a specific index in the contour
TEST(ContourTests, AddItemAt) {
    Contour contour;

    // Add initial elements
    Line2 line1(Point2{ 0, 0 }, Point2{ 1, 1 });
    Line2 line2(Point2{ 1, 1 }, Point2{ 2, 2 });
    contour.addItem(line1);
    contour.addItem(line2);

    // Add a new element at index 1
    Line2 newLine(Point2{ 0.5, 0.5 }, Point2{ 1.5, 1.5 });
    contour.addItemAt(std::move(newLine), 1);

    // Verify the contour has three elements
    EXPECT_EQ(contour.getElements().size(), 3) << "Contour should have three elements after adding an item.";

    // Verify the order of elements
    auto elements = contour.getElements();
    const Line2& first_Line = std::get<Line2>(elements[0]);
    const Line2& inserted_Line = std::get<Line2>(elements[1]);
    const Line2& lastLine = std::get<Line2>(elements[2]);

    EXPECT_EQ(first_Line.getCoordinate(0).x, 0);
    EXPECT_EQ(first_Line.getCoordinate(0).y, 0);
    EXPECT_EQ(first_Line.getCoordinate(1).x, 1);
    EXPECT_EQ(first_Line.getCoordinate(1).y, 1);

    EXPECT_EQ(inserted_Line.getCoordinate(0).x, 0.5);
    EXPECT_EQ(inserted_Line.getCoordinate(0).y, 0.5);
    EXPECT_EQ(inserted_Line.getCoordinate(1).x, 1.5);
    EXPECT_EQ(inserted_Line.getCoordinate(1).y, 1.5);

    EXPECT_EQ(lastLine.getCoordinate(0).x, 1);
    EXPECT_EQ(lastLine.getCoordinate(0).y, 1);
    EXPECT_EQ(lastLine.getCoordinate(1).x, 2);
    EXPECT_EQ(lastLine.getCoordinate(1).y, 2);

    // Test out-of-bounds index
    EXPECT_THROW(contour.addItemAt(Line2(Point2{ 3, 3 }, Point2{ 4, 4 }), 10), std::out_of_range);
}

// Test for adding an item to the center of the contour
TEST(ContourTests, AddItemToCenter) {
    Contour contour;

    // Add initial elements
    Line2 line1(Point2{0, 0}, Point2{1, 1});
    Line2 line2(Point2{2, 2}, Point2{3, 3});
    contour.addItem(line1);
    contour.addItem(line2);

    // Add a new element to the center
    ContourElement new_line = Line2(Point2{ 1, 1 }, Point2{ 2, 2 });
    contour.addItemToCenter(new_line);

    // Verify the contour has three elements
    EXPECT_EQ(contour.getElements().size(), 3) << "Contour should have three elements after adding an item to the center.";

    // Verify the order of elements
    auto elements = contour.getElements();
    const Line2& first_line = std::get<Line2>(elements[0]);
    const Line2& center_line = std::get<Line2>(elements[1]);
    const Line2& last_line = std::get<Line2>(elements[2]);

    EXPECT_EQ(first_line.getCoordinate(0).x, 0);
    EXPECT_EQ(first_line.getCoordinate(0).y, 0);
    EXPECT_EQ(first_line.getCoordinate(1).x, 1);
    EXPECT_EQ(first_line.getCoordinate(1).y, 1);

    EXPECT_EQ(center_line.getCoordinate(0).x, 1);
    EXPECT_EQ(center_line.getCoordinate(0).y, 1);
    EXPECT_EQ(center_line.getCoordinate(1).x, 2);
    EXPECT_EQ(center_line.getCoordinate(1).y, 2);

    EXPECT_EQ(last_line.getCoordinate(0).x, 2);
    EXPECT_EQ(last_line.getCoordinate(0).y, 2);
    EXPECT_EQ(last_line.getCoordinate(1).x, 3);
    EXPECT_EQ(last_line.getCoordinate(1).y, 3);
}

// Test for unique contours
TEST(VectorContoursUniquenessTests, UniqueContours) {
    // Create unique contours
    Contour contour1, contour2, contour3;

    contour1.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    contour2.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));
    contour3.addItem(Line2(Point2{ 2, 2 }, Point2{ 3, 3 }));

    std::vector<Contour> contours = { contour1, contour2, contour3 };

    // Verify that the contours are unique
    EXPECT_TRUE(vectorContoursUniqueness(contours)) << "Contours should be unique.";
}

// Test for duplicate contours
TEST(VectorContoursUniquenessTests, DuplicateContours) {
    // Create contours with duplicates
    Contour contour1, contour2, contour3;

    contour1.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));
    contour2.addItem(Line2(Point2{ 1, 1 }, Point2{ 2, 2 }));
    contour3.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 })); // Duplicate of contour1

    std::vector<Contour> contours = { contour1, contour2, contour3 };

    // Verify that the contours are not unique
    EXPECT_FALSE(vectorContoursUniqueness(contours)) << "Contours should not be unique.";
}

// Test for an empty vector of contours
TEST(VectorContoursUniquenessTests, EmptyContours) {
    std::vector<Contour> contours;

    // Verify that an empty vector is considered unique
    EXPECT_TRUE(vectorContoursUniqueness(contours)) << "Empty vector of contours should be unique.";
}

// Test for a single contour
TEST(VectorContoursUniquenessTests, SingleContour) {
    Contour contour1;
    contour1.addItem(Line2(Point2{ 0, 0 }, Point2{ 1, 1 }));

    std::vector<Contour> contours = { contour1 };

    // Verify that a single contour is considered unique
    EXPECT_TRUE(vectorContoursUniqueness(contours)) << "A single contour should be unique.";
}