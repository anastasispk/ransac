cmake_minimum_required(VERSION 3.12)
project(ransac)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 11)

# Add include directory
include_directories(lib)
add_executable(ransac src/main.cpp)

# target_include_directories(main PRIVATE ${MATPLOTLIB_CPP_INCLUDE_DIRS})
find_package(Python3 COMPONENTS Development NumPy)
target_link_libraries(ransac Python3::Python)
target_include_directories(ransac PRIVATE ${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

# Add your source files

# Create the executable
target_link_libraries(ransac)