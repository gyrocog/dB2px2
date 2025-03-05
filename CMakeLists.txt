cmake_minimum_required(VERSION 3.10)
project(dB2px2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)
add_executable(dB2px2 WIN32 main.cpp db2px2.ico)
target_link_libraries(dB2px2 PRIVATE wx::core wx::base)

# Copy icon to build directory
configure_file(${CMAKE_SOURCE_DIR}/db2px2.ico ${CMAKE_BINARY_DIR}/Release/db2px2.ico COPYONLY)