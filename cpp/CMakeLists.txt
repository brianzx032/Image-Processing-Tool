INCLUDE_DIRECTORIES(DIR_SRC ./src)
cmake_minimum_required(VERSION 2.8)
project( MyCvProject )
find_package( OpenCV REQUIRED )
# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})
AUX_SOURCE_DIRECTORY(DIR_SRC ./src)
add_executable( MyCvProject main.cpp ./src/cv2ImageProcessing.cpp)
target_link_libraries( MyCvProject ${OpenCV_LIBS} ${GTK3_LIBRARIES})
