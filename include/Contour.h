#pragma once
#ifndef CONTOUR_H
#define CONTOUR_H

#include <Config.h>

#include <Arc.h>
#include <Line2.h>
#include <vector>
#include <variant>
#include <string>
#include <shared_mutex>
#include <stdexcept>

#include "Line2.h"
#include "Arc.h"

using ContourElement = std::variant<Line2, Arc>;
/* For easy extension of the library, we use a variant, introduced in c++17.
 * Just add your class that "extends Segment" and implement the methods and overrides
 * and you should be good to go.
 */

class Contour {  /*!< A Contour is either a Line2 or an Arc. The class has several public methods for comparison, moving copying and debugging (svg) */
public:
	Contour() = default;
	Contour(const Contour& other);
	Contour(Contour&& other) noexcept;
	Contour& operator=(const Contour& other);
	Contour& operator=(Contour&& other) noexcept;

	bool operator==(const Contour& other) const;
	void addItem(ContourElement item);
	void addItemToCenter(ContourElement& item);
	bool isValid() const;
	std::vector<ContourElement> getElements() const;
	std::vector<Point2> getLineStrip() const;

	void clear();
	void clearAtIndex(int index);

	void exportContourToSVG(const std::string& filename) const;
	void print(const std::string& padding) const;

private:
	bool computeValidity() const;

	mutable std::shared_mutex _mutex;
	std::vector<ContourElement> _elements;
	mutable bool is_valid_dirty_ = true;
	mutable bool is_valid_cache_ = false;
};

// Utility functions

void printPoints(const std::vector<Point2>& v);

// Create a contour consisting only of Line2s from a list of points
Contour contourFromPoints(const std::vector<Point2>& pts);

// Brute force uniqueness check
bool vectorContoursUniqueness(const std::vector<Contour>& contours);

// Check if two contour vectors contain the same elements, obsolete
bool vectorsEqualContour(const std::vector<Contour>& a, const std::vector<Contour>& b);

// Filter contours based on validity
void filterValidStateContour(const std::vector<Contour>& contours, std::vector<Contour>& output, bool validState);

#endif // CONTOUR_H
