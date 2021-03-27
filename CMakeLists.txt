cmake_minimum_required(VERSION 3.17.2)

project(ohayou-cli VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#third-party deps
find_package(OpenCV 4.5.1 REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "./includes")

#link third-party deps
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries( ${PROJECT_NAME} PRIVATE harfbuzz::harfbuzz)

