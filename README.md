# Contour Project
<p align="center">
  <img src="https://github.com/user-attachments/assets/69d5e11d-e8b0-4f0a-8dfb-095e74a7e70b" alt="Lorentz attractor plot" width="400"/>
</p>

## Overview

This project implements a 2D Contour class that can store and manipulate a sequence of connected segments.

It is written in C++17 using Visual Studio 2022 and CMake. 

## Build Instructions
Run **build.bat**, it will call **cmake** and create the build directory along with **ContourProjectMain**, **ContourLib** and **ContourTests**. **ContourLib** is generated so we can reference it from the other projects. Open the Visual Studio **ContourProject.sln** file in Build. Gtest is used and downloaded when building with cmake.

NOTE: Running main.cpp in ContourProjectMain will create a local file **test-lorentz.svg**.

## Definition
A **Contour** consists of a vector of items where items can be **Lines** or **Arcs**. Everything is in 2D. The project is written in a way that future extension for additional segment types is easily added. The Contour class is designed to support flexible construction and validation, while maintaining high performance through caching (**Contour::isValid**) and avoiding unnecessary calculations (we do not calculate **sqrt** for distance for instance). Additional performance can be achieved using vectorization, which is not explored at this time.

Config.h contains macros for constants and things that affect the whole project, for instance EPS and RES. Config includes the headers of the project and it turn their headers read config.h.

### 🔬 Testing

The contour is considered valid if all consecutive segments are connected end-to-begin within a small epsilon tolerance (see in config.h **EPS**).
Validation is calculated on demand and cached to avoid redundant computations. 
Contours are validated using the **isValid()** function to ensure proper segment connectivity. Tests are divided into files for each category.
Read more about it https://learn.microsoft.com/en-us/visualstudio/test/improve-code-quality?view=vs-2022

![gtests](https://github.com/user-attachments/assets/68dff55e-e18c-44c7-9cce-6d3442afeb0e)

### Utility Functions:

Function for creating a Contour from a series of points interpreted as a polyline (**contourFromPoints**).

### Write to SVG file
To debug the contours you might want to export them to SVG format and open them using InkScape.
Some shapes (contours) were generated by the test function. It shows contours of arcs only, lines only and a mix of the two.

<p align="center">
  <img src="https://github.com/user-attachments/assets/58e077b6-c0c0-4685-84be-dafce6b8633d" alt="shapes" width="400"/>
</p>

## Future Improvements
- [ ] Doxygen: Configure Doxygen for automated documentation generation.

### Additional Segment Types:

- [ ] B-splines
- [ ] clothoids
- [ ] polygons as new segment type.
- [ ] Check if a point is inside a polygon
- [ ] Investigate performance **virtual** versus **Template**

### Advanced Contour Operations:

- [ ] Resampling
- [ ] **simplification** (FFT?)
- [ ] Turtle graphics tape (inspiration from my course ARK385)
- [ ] L System?

- [ ] Investigate optimization (SSE, assembly, BSP)

### Contour Visualization:

- [ ] Integrated Qt for contour visualization, aiding in debugging and inspection. (high priority)
