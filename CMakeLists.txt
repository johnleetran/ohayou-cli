cmake_minimum_required(VERSION 3.17.2)

project(ohayou-cli VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE MATCHES Release)
    message("optimize code")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

#third-party deps
find_package(OpenCV 4.5.1 REQUIRED)
find_package(harfbuzz CONFIG REQUIRED)
pkg_check_modules(VipsCpp REQUIRED IMPORTED_TARGET vips-cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "./includes")
target_include_directories(${PROJECT_NAME} PUBLIC ${VipsCpp_INCLUDE_DIRS})
#link third-party deps
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries( ${PROJECT_NAME} PRIVATE harfbuzz::harfbuzz)
target_link_libraries( ${PROJECT_NAME} PRIVATE PkgConfig::VipsCpp)

set(CMAKE_C_FLAGS "${VipsCpp_CFLAGS_OTHER} ")

message(INFO "<<< VipsCpp_LIBRARIES << ${VipsCpp_LIBRARIES}")
message(INFO "<<< VipsCpp_INCLUDE_DIRS << ${VipsCpp_INCLUDE_DIRS}")
message(INFO "<<< VipsCpp_CFLAGS << ${VipsCpp_CFLAGS}")
message(INFO "<<< VipsCpp_CFLAGS_OTHER << ${VipsCpp_CFLAGS_OTHER}")
