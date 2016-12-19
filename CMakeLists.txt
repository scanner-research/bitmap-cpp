cmake_minimum_required(VERSION 3.2)

project(BitmapCpp)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src)

file(GLOB_RECURSE SRC RELATIVE ${PROJECT_SOURCE_DIR} bitmap-cpp/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} bitmap-cpp/*.h)

add_library(bitmapcpp ${HEADERS} ${SRC})

install(TARGETS bitmapcpp
  EXPORT BitmapCppTarget
  ARCHIVE DESTINATION lib)

set(BITMAPCPP_LIBRARY "${CMAKE_INSTALL_PREFIX}/lib/libbitmapcpp.a")
set(BITMAPCPP_LIBRARIES ${CUSTOM_LIBRARIES})

configure_file(cmake/BitmapCppConfig.cmake.in
  "${PROJECT_BINARY_DIR}/cmake/BitmapCppConfig.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/cmake/BitmapCppConfig.cmake"
  DESTINATION cmake)

install(EXPORT BitmapCppTarget
  DESTINATION cmake)

install(DIRECTORY bitmap-cpp/ DESTINATION include/bitmap-cpp
  FILES_MATCHING PATTERN "*.h")
