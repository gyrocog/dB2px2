# dB2px2
A simple cross-platform tool to convert dB values to pixel counts for VU meter design.

## Purpose
This app calculates a range of dB values and their corresponding pixel counts based on user inputs: Maximum Volume (dB), List to (-dB), Precision (dB), and VU Meter Size (px). Results are displayed in a table.

## Requirements
- **CMake**: Build system (install from [cmake.org](https://cmake.org)).
- **wxWidgets**: GUI library (install via vcpkg: see below).
- **vcpkg**: Dependency manager (install from [github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg)).

## Build Instructions
1. **Clone the Repository**:

git clone https://github.com/gyrocog/dB2px2.git

cd dB2px2

2. **Install Dependencies** (assuming vcpkg is installed):

vcpkg install wxwidgets:x64-windows

3. **Build**:

mkdir build

cd build

cmake .. -A x64 -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake

cmake --build . --config Release

4. **Run**:

Release\dB2px2.exe


## Notes
- Replace `C:\path\to\vcpkg` with your actual vcpkg installation path.
- Built with wxWidgets for a native GUI experience on Windows.

## License
Free to copy, modify... do whatever you want with it.