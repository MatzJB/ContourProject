#include <Config.h>

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <fstream>


// TODO: add 2x2 matrix feature with scaling, translation and rotation

Contour::Contour(const Contour& other)
{
	std::shared_lock lock(other._mutex);
	_elements = other._elements;
	is_valid_dirty_ = other.is_valid_dirty_;
	is_valid_cache_ = other.is_valid_cache_;
}

Contour::Contour(Contour&& other) noexcept
{
	std::unique_lock lock(other._mutex);
	_elements = std::move(other._elements);
	is_valid_dirty_ = other.is_valid_dirty_;
	is_valid_cache_ = other.is_valid_cache_;
}

Contour& Contour::operator=(const Contour& other)
{
	if (this != &other)
	{
		std::unique_lock lock_this(_mutex, std::defer_lock);
		std::shared_lock lock_other(other._mutex, std::defer_lock);
		std::lock(lock_this, lock_other);

		_elements = other._elements;
		is_valid_dirty_ = other.is_valid_dirty_;
		is_valid_cache_ = other.is_valid_cache_;
	}
	return *this;
}

Contour& Contour::operator=(Contour&& other) noexcept
{
	if (this != &other)
	{
		std::unique_lock lock_this(_mutex, std::defer_lock);
		std::unique_lock lock_other(other._mutex, std::defer_lock);
		std::lock(lock_this, lock_other);

		_elements = std::move(other._elements);
		is_valid_dirty_ = other.is_valid_dirty_;
		is_valid_cache_ = other.is_valid_cache_;
	}
	return *this;
}

bool Contour::operator==(const Contour& other) const
{
	const auto& elems_a = getElements();
	const auto& elems_b = other.getElements();
	if (elems_a.size() != elems_b.size()) return false;
	for (size_t i = 0; i < elems_a.size(); ++i)
		if (!(elems_a[i] == elems_b[i])) return false;
	return true;
}

//TODO: Check that lvalue is easier to use here
void Contour::addItem(ContourElement item)
{
	std::unique_lock lock(_mutex);
	_elements.emplace_back(item);
	is_valid_dirty_ = true;
}

void Contour::addItemAt(ContourElement&& item, unsigned int index)
{
	std::unique_lock lock(_mutex);
	if (index > _elements.size())
	{
		throw std::out_of_range("Index is out of bounds");
	}
	_elements.insert(_elements.begin() + index, std::move(item));
	is_valid_dirty_ = true;
}

// TODO: edge cases?
void Contour::addItemToCenter(const ContourElement& item)
{
	std::unique_lock lock(_mutex);
	auto middle = _elements.begin() + _elements.size() / 2;
	_elements.insert(middle, item);
	is_valid_dirty_ = true;
}

// A Contour is valid if the distance between all internal consecutive 2D points are less than EPS.
// For a contour with only one element and only two Point2 it is valid.
// It is assumed that all segments are valid.
// TODO: Should I check the validity of the segments?
bool Contour::isValid() const
{
	{
		std::shared_lock read_lock(_mutex);
		if (!is_valid_dirty_)
		{
			return is_valid_cache_;
		}
	}
	// Upgrade to write lock
	std::unique_lock write_lock(_mutex);
	if (!is_valid_dirty_)
	{
		return is_valid_cache_;
	}
	is_valid_cache_ = computeValidity();
	is_valid_dirty_ = false;
	return is_valid_cache_;
}

std::vector<ContourElement> Contour::getElements() const
{
	std::shared_lock lock(_mutex);
	return _elements;
}

void Contour::clear()
{
	std::unique_lock lock(_mutex);
	_elements.clear();
	is_valid_dirty_ = true;
}

void Contour::clearAtIndex(int index)
{
	std::unique_lock lock(_mutex);
	if (index >= 0 && index < static_cast<int>(_elements.size()))
	{
		_elements.erase(_elements.begin() + index);
		is_valid_dirty_ = true;
	}
	else
	{
		throw std::out_of_range("Index is out of bounds");
	}
}

// Please note, Line2 strip resolution only makes sense for non-Line2 objects.
std::vector<Point2> Contour::getLineStrip() const
{
	std::vector<Point2> result;
	std::shared_lock lock(_mutex);
	for (const auto& e : _elements)
	{
		std::visit([&](const auto& element)
		{
			auto poly = element.getLineStrip();
			result.insert(result.end(), poly.begin(), poly.end());
		}, e);
	}
	return result;
}


/* Function to export the contour to an SVG file.
 * <Resolution> and <scale> are optional parameters.
 * Resolution overrides Arcs. Line2s are unaffected by design. */
// TODO: add resolution to arcs

void Contour::exportContourToSVG(const std::string& filename) const
{
	std::ofstream file(filename);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file.");
	}

	file << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"600\" height=\"600\" viewBox=\"-100 -100 600 600\">\n";
	file << "<g fill=\"none\" stroke=\"black\" stroke-width=\"2\" stroke-Line2join=\"round\">\n";
	double scale = 10;
	const auto elements = this->getElements();
	std::string pathData;

	for (const auto& e : elements)
	{
		std::vector<Point2> pts;
		std::visit([&](const auto& element)
		{
			pts = element.getLineStrip();
		}, e);

		for (size_t j = 0; j < pts.size(); ++j)
		{
			pathData += (j == 0 ? "M " : "L ") +
				std::to_string(scale * pts[j].x) + " " +
				std::to_string(scale * (1 - pts[j].y)) + " ";
		}
	}

	file << "<path d=\"" << pathData << "\" />\n</g>\n</svg>\n";
	file.close();
}

void Contour::print(const std::string& padding) const
{
	std::shared_lock lock(_mutex);
	std::cout << padding << "Contour with " << _elements.size() << " segments:\n";

	for (const auto& e : _elements)
	{
		std::visit([padding](const auto& segment)
		{
			segment.print(padding);
		}, e);
	}
}

bool Contour::computeValidity() const
{
	if (this->_elements.size() < 2) return true;

	auto get_point = [](const auto& element, double t)
	{
		return std::visit([t](const auto& seg)
		{
			return seg.getCoordinate(t);
		}, element);
	};

	// Check if the distance between consecutive points is less than EPS
	for (size_t i = 0; i < _elements.size() - 1; ++i)
	{
		Point2 end = get_point(_elements[i], 1.0);
		Point2 start = get_point(_elements[i + 1], 0.0);

		if (!start.isCloseTo(end, EPS))
		{
			return false;
		}
	}
	// all internal points are close enough
	return true;
}


// Returns a contour consisting of Line2s from a vector of Point2s
Contour contourFromPoints(const std::vector<Point2>& pts)
{
	if (pts.size() < 2)
	{
		throw std::invalid_argument("At least two points are required to create a contour.");
	}
	Contour c;
	for (size_t i = 0; i + 1 < pts.size(); ++i)
	{
		Line2 l({pts[i], pts[i + 1]});
		c.addItem(l);
	}

	return c;
}


// Brute force uniqueness check
bool vectorContoursUniqueness(const std::vector<Contour>& contours)
{
	for (size_t i = 0; i < contours.size(); ++i)
	{
		for (size_t j = i + 1; j < contours.size(); ++j)
		{
			if (contours[i] == contours[j])
			{
				return false;
			}
		}
	}
	return true;
}

// TODO: Obsolete, remove?
//static bool vectorsEqualContour(const std::vector<Contour>& a, const std::vector<Contour>& b) {
//
//	if (vectorContoursUniqueness(a) || vectorContoursUniqueness(b))
//		return false;
//
//	for (const auto& ca : a) {
//		bool found = false;
//		for (const auto& cb : b) {
//			if (ca == cb) {
//				found = true;
//				break;
//			}
//		}
//		if (!found) return false;
//	}
//	return true;
//}

// Filter contours based on their validityState.
void filterValidStateContour(const std::vector<Contour>& contours, std::vector<Contour>& output, bool validState)
{
	for (const auto& c : contours)
	{
		if (c.isValid() == validState)
		{
			output.emplace_back(c); // this is fine, since all reading and writing are under mutex locks in contour
		}
	}
}
