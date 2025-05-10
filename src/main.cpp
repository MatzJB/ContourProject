#include <iostream>
#include <string>  
#include <variant>
#include <vector>
#include "Config.h"
#include <Contour.h>
#include "Point2.h"


int main() {
    // Lorentz attaractor
    const double rho = 28.0;
    const double beta = 8.0 / 3.0;
    const double sigma = 10.0;
    // Time span
    const double dt = 0.01;
    const int num_steps = 8000;

    // Preallocate arrays for x, y, z
    std::vector<double> x(num_steps, 0.0);
    std::vector<double> y(num_steps, 0.0);
    std::vector<double> z(num_steps, 0.0);

    // Initial conditions
    x[0] = 1.0;
    y[0] = 1.0;
    z[0] = 1.0;

    // Euler integration
    for (int i = 0; i < num_steps - 1; ++i) {
        double dx = sigma * (y[i] - x[i]);
        double dy = x[i] * (rho - z[i]) - y[i];
        double dz = x[i] * y[i] - beta * z[i];

        x[i + 1] = x[i] + dx * dt;
        y[i + 1] = y[i] + dy * dt;
        z[i + 1] = z[i] + dz * dt;
    }

    // Create a Contour to represent the trajectory
    Contour trajectory;

    // Add Line2 segments to the Contour
    for (int i = 0; i < num_steps - 1; ++i) {
        trajectory.addItem(Line2(
            Point2({ x[i], y[i] }),
            Point2({ x[i + 1], y[i + 1] })));
    }

    // Warning: writes to the current working directory
    std::string filename = "test-lorentz.svg";

    trajectory.exportContourToSVG(filename);
    std::cout << "wrote file " << filename << "\n";
}
