cmake_minimum_required(VERSION 3.17.2)

project(ohayou-cli VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
if(CMAKE_BUILD_TYPE MATCHES Release)
    message("optimize code")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

#third-party deps
find_package(OpenCV 4.5.1 REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
# disable VIPS until i figure out what to do with it
# pkg_check_modules(VipsCpp REQUIRED IMPORTED_TARGET vips-cpp)
# set(CMAKE_C_FLAGS "${VipsCpp_CFLAGS_OTHER} ")

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "./includes")
# target_include_directories(${PROJECT_NAME} PUBLIC ${VipsCpp_INCLUDE_DIRS})
#link third-party deps
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries( ${PROJECT_NAME} PRIVATE harfbuzz::harfbuzz)
# target_link_libraries( ${PROJECT_NAME} PRIVATE PkgConfig::VipsCpp)



