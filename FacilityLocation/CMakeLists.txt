﻿# CMakeList.txt : CMake project for FacilityLocation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories("headers" "sources")

file(GLOB source_files "headers/*.h" "sources/*.cpp")

# Add source to this project's executable.
add_executable (FacilityLocation "FacilityLocation.cpp" ${source_files})

# TODO: Add tests and install targets if needed.
