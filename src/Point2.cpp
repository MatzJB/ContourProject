#include "Config.h"
#include "Point2.h"

#include <unordered_map>
#include <vector>

/*
 * As this is a toy example and to make things simple, I use a struct because it is
 *  faster because it is allocated on the Stack, as opposed to the Heap.
 *  If we want to use this in a double world example, we might want to consider
 *  vectorizing the code (SSE), by storing the coordinates in separate vectors.
 */

Point2 Point2::operator+(const Point2& point) const
{
	return { x + point.x, y + point.y };
}

Point2 Point2::operator-(const Point2& point) const
{
	return { x - point.x, y - point.y };

}

bool Point2::operator==(const Point2& point) const
{
	return fabs(x - point.x) < EPS && fabs(y - point.y) < EPS;
}

// Fast check of distance between points, purposely avoiding using sqrt
bool Point2::isTooFarTo(const Point2& point, double threshold) const
{
	return (x - point.x) * (x - point.x) +
		(y - point.y) * (y - point.y) < threshold * threshold;
}

static std::vector<Point2> generatePointsLSystem(std::string instructions, double step_length, double angle_step, const std::unordered_map<char, std::string>& rules) {
    std::vector<Point2> points;
    double angle = 0.0f;
    Point2 current_position(Point2({ 0.0f, 0.0f }));

    points.push_back(current_position); // Add starting point

    // Traverse the L-System string
    for (size_t i = 0; i < instructions.length(); ++i) {
        char current_character = instructions[i];

        if (current_character == 'F') {
            // Move forward by stepLength and create a new point
            Point2 new_position = current_position;
            new_position.x += step_length * cos(angle);
            new_position.y += step_length * sin(angle);
            points.push_back(new_position);
            current_position = new_position;
        }
        else if (current_character == '+') {
            // Rotate clockwise by the specified angle
            angle -= angle_step * PI;
        }
        else if (current_character == '-') {
            // Rotate counter-clockwise by the specified angle
            angle += angle_step * PI;
        }
        else if (rules.find(current_character) != rules.end()) {
            // If the character has a replacement rule, apply it
            std::string replacement = rules.at(current_character);
            for (char rule_character : replacement) {
                // Add each character from the replacement string to the instructions
                instructions.push_back(rule_character);
            }
        }
    }

    return points;
}

