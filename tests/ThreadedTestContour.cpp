#include "gtest/gtest.h"
#include "Contour.h"
#include "Point2.h"
#include "Line2.h"
#include "Arc.h"
#include <thread>
TEST(ContourThreadedValidationTest, FilterValidAndInvalidContoursCorrectly)
{
    // Setup
    Contour contour1; // valid contour
    Contour contour2; // invalid contour
    Contour contour3; // invalid contour
    Contour contour4; // valid contour

    // Create "Periodic half circles" - valid
    int resolution = 200;
    Arc a1(Point2({ 0, 0 }), 1, 0, PI, resolution);
    Arc a2(Point2({ -2, 0 }), 1, 2 * PI, PI, resolution, false);
    Arc a3(Point2({ -4, 0 }), 1, 0, PI, resolution);
    Arc a4(Point2({ -6, 0 }), 1, 0, PI, resolution, false);
    contour1.addItem(a1);
    contour1.addItem(a2);
    contour1.addItem(a3);
    contour1.addItem(a4);


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
