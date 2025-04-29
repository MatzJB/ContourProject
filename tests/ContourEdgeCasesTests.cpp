#include "gtest/gtest.h"  
#include "Contour.h"  
#include "Point2.h"  
#include "Arc.h"  
#include "Line2.h"  

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
   Line2 Line(Point2({ 0, 0 }), Point2({ 1, 1 }));  
   contour.addItem(Line);  

   // A single-element contour should be valid  
   EXPECT_TRUE(contour.isValid());  

   // The number of elements should be one  
   EXPECT_EQ(contour.getElements().size(), 1);  
}  

TEST(ContourEdgeCases, AddItemToCenter) {  
   Contour contour;  

   // Add initial elements  
   contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));  
   contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));  

   // Add an element to the center  
   Line2 newLine2(Point2({ 0.5, 0.5 }), Point2({ 1.5, 1.5 }));  
   contour.addItemToCenter((ContourElement)newLine2);  

   // Check the number of elements  
   EXPECT_EQ(contour.getElements().size(), 3);  

   // Check that the new element is in the center  
   auto elements = contour.getElements();  
   Line2 centerLine2 = std::get<Line2>(elements[1]);  

   EXPECT_EQ(newLine2, centerLine2);  
}  

TEST(ContourEdgeCases, ClearAtInvalidIndex) {  
   Contour contour;  

   // Add some elements  
   contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));  

   // Attempt to clear an invalid index  
   EXPECT_THROW(contour.clearAtIndex(5), std::out_of_range);  
   EXPECT_THROW(contour.clearAtIndex(-1), std::out_of_range);  
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

TEST(ContourEdgeCases, MixedValidity) {
    Contour contour;

    // Add valid elements
    contour.addItem(Line2(Point2({ 0, 0 }), Point2({ 1, 1 })));
    contour.addItem(Line2(Point2({ 1, 1 }), Point2({ 2, 2 })));

    // Add an invalid element
    contour.addItem(Line2(Point2({ 10, 10 }), Point2({ 11, 11 }))); // Too far from the previous Line2

    // The contour should be invalid
    EXPECT_FALSE(contour.isValid());
}

TEST(ContourEdgeCases, ArcWithZeroResolution) {
    Contour contour;

    // Add an arc with zero resolution and expect an exception
    EXPECT_THROW(
        contour.addItem(Arc(Point2({ 0, 0 }), 1, 0, PI, 0)),
        std::invalid_argument
    ) << "Adding an Arc with zero resolution should throw an invalid_argument exception.";
}

TEST(ContourEdgeCases, DuplicateElements) {
    Contour contour;

    // Add duplicate elements
    Line2 Line2(Point2({ 0, 0 }), Point2({ 1, 1 }));
    contour.addItem(Line2);
    contour.addItem(Line2);

    // The contour should be invalid
    EXPECT_FALSE(contour.isValid());
    
    // The number of elements should be two
    EXPECT_EQ(contour.getElements().size(), 2);
}
