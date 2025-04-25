/*
 * ContourProject
 * Author: Matz JB
 *
 * Todo:
 *  * add tests with Google Test
 *  * Fix comparisons
 *  * Fix style
 *  * Fix google test header include
 *  * write cmake
 *  * doxygen or something to create documentation
 */

#include <iostream>
#include <string>  
#include <variant>
#include <vector>
#include <mutex>

#include "Config.h"
#include <Contour.h>

#include "Point2.h"
#include <Arc.h>
 //#include "../packages/Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn.1.8.1.7/build/native/include/gtest/gtest.h"


 //https://google.github.io/styleguide/cppguide.html


// Tests, replace with Google tests
static void contourTests()
{
	// expected test outcome
	int valid_expected = 2;
	int invalid_expected = 2;

	Contour contour1; // valid contour
	Contour contour2; // invalid contour
	Contour contour3; // invalid contour
	Contour contour4; // valid contour

	// "Periodic half circles" - valid contour
	Arc a1(Point2({ 0, 0 }), 1, 0, PI, true); // note: backwards for 0 to pi
	Arc a2(Point2({ 2, 0 }), 1, PI, 2 * PI);
	Arc a3(Point2({ 4, 0 }), 1, 0, PI, true);
	Arc a4(Point2({ 6, 0 }), 1, PI, 2 * PI);


	contour1.addItem(a1);
	contour1.addItem(a2);
	contour1.addItem(a3);
	contour1.addItem(a4);
	//contour1.export_contour_to_svg("c:\\temp\\contour1.svg");

	// "Lines" - Invalid contour
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
	//contour2.export_contour_to_svg("c:\\temp\\contour2.svg");

	// capsule - invalid contour
	Arc c1(Point2({ 0, 0 }), 1, 0, PI);
	Line c3(Point2({ -1, 0 }), Point2({ -1, -3 }));
	Arc c2(Point2({ 0, -3 }), 1, PI, 2 * PI);
	Line c4(Point2({ 1 + EPS, -3 }), Point2({ 1, 0 }));

	contour3.addItem(c1);
	contour3.addItem(c3);
	contour3.addItem(c2);
	contour3.addItem(c4);
	//contour3.export_contour_to_svg("c:\\temp\\contour3.svg");

	// Rectangle - valid contour
	Line d1(Point2({ 0, 0 }), Point2({ 10, 0 }));
	Line d2(Point2({ 10, 0 }), Point2({ 10, 10 + 0.9 * EPS })); // almost disconnected but within EPS!
	Line d3(Point2({ 10, 10 }), Point2({ 0, 10 }));
	Line d4(Point2({ 0, 10 }), Point2({ 0, 0 }));

	contour4.addItem(d1);
	contour4.addItem(d2);
	contour4.addItem(d3);
	contour4.addItem(d4);
	//contour4.export_contour_to_svg("c:\\temp\\contour4.svg");

	std::vector<Contour> contours;
	contours.emplace_back(contour1);
	contours.emplace_back(contour2);
	contours.emplace_back(contour3);
	contours.emplace_back(contour4);

	// Thread tests
	std::vector<Contour> validContours;
	std::vector<Contour> invalidContours;

	// filter contour vector into valid and invalid states
	std::thread thread_1(filterValidStateContour, std::cref(contours), std::ref(validContours), true);
	std::thread thread_2(filterValidStateContour, std::cref(contours), std::ref(invalidContours), false);

	std::cout << " Created threads\n";
	std::cout << "  ID " << thread_1.get_id() << "\n";
	std::cout << "  ID " << thread_2.get_id() << "\n";

	thread_1.join();
	thread_2.join();

	std::string padding = "      ";
	printf("\n\n\n  Valid contours:");
	for (int i = 0;i < validContours.size();i++)
	{
		printf("\n  Contour %d:\n", i);
		validContours[i].print(padding);
	}
	printf("  Invalid contours:");

	for (int i = 0;i < invalidContours.size();i++)
	{
		printf("\n  Contour %d:\n", i);
		invalidContours[i].print(padding);
	}


	printf("\n\n Results:\n");


	// check that the answers are correct
	int valid_result = 0;
	int invalid_result = 0;
	printf("  Testing Valid contours...\n");

	for (const auto& c : validContours) {
		if (c.isValid()) {
			valid_result++;
		}
	}

	printf("   %d out of %d tests were correctly identified\n", valid_result, valid_expected);

	if (valid_result != valid_expected) {
		printf("!!Something went wrong, please reach out to the developer to fix the code.\n");

	}
	printf("  Testing Invalid contours...\n");

	for (const auto& c : invalidContours) {
		if (!c.isValid()) {
			invalid_result++;
		}
	}
	printf("   %d out of %d tests were correctly identified\n", invalid_result, invalid_expected);

	if (valid_result != valid_expected) {
		printf("!!Something went wrong, please reach out to the developer to fix the code.\n");
	}

	// Join the contours
	std::vector<Contour> joined = validContours;
	joined.insert(joined.end(), invalidContours.begin(), invalidContours.end());

	// Check that all elements are unique (as far as pointers are concerned, not the segments themselves)
	printf("  Testing for uniqueness in joined Contour vector...\n");

	if (vectorContoursUniqueness(joined))
	{
		printf("   Both contour vectors were separated using threads to pick out validity state, executed and merged into one vector and this vector seems to be unique\n");
	}
	else
	{
		printf("ERROR: Something went wrong with the tests.\n");
		printf(" Possible causes:");
		printf("  Comparison operation of Contours is wrong\n");
		printf("  Shared mutex is not properly used\n");
		printf("  EPS is set too high\n");
		printf("  The Line/Arc code is incorrect\n");
	}

	printf(" Done\n");
}



//
//int main(int argc, char** argv) {
//	::testing::InitGoogleTest(&argc, argv);
//	return RUN_ALL_TESTS();
//}/*

int main(int argc, char** argv) {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(print_precision); // flushing prints will hit performance


	printf("Performing tests...\n\n");

	try {
		contourTests();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	// Task 9
	//std::vector<Point_2> tmp;
	//tmp.emplace_back(Point_2(0, 0));
	//tmp.emplace_back(Point_2(1, 1));
	//tmp.emplace_back(Point_2(5, 6));
	//tmp.emplace_back(Point_2(10, 10));
	//tmp.emplace_back(Point_2(15, 15));
	//tmp.emplace_back(Point_2(15, 25));

	//Contour contour = contourFromCoordinates(tmp);
	//contour.exportContourToSVG("c:\\temp\\contour5.svg");
}