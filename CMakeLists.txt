cmake_minimum_required(VERSION 3.2)

project(BitmapCpp)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_definitions(-std=c++11)

set(CMAKE_MODULE_PATH ${CMAKE_HOME_DIRECTORY}/cmake ${CMAKE_MODULE_PATH})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(HALIDE_TARGETS)
macro(add_halide_target SRC TARGET)
  if (NOT HALIDE_FOUND)
    find_package(Halide REQUIRED)
    include(${CMAKE_SOURCE_DIR}/cmake/Util/HalideGenerator.cmake)
    include_directories("${HALIDE_INCLUDE_DIR}")
  endif()

  get_filename_component(NAME ${SRC} NAME_WE)
  set(GENERATOR ${NAME}.generator)
  halide_add_generator(${GENERATOR}
    SRCS ${SRC})
  halide_add_aot_library(${NAME}
    GENERATOR_TARGET ${GENERATOR}
    GENERATOR_ARGS target=${TARGET})
  list(APPEND HALIDE_TARGETS ${NAME})
endmacro()

add_halide_target(bitmap-cpp/bitmap_transformer.cpp host)

file(GLOB_RECURSE SRC RELATIVE ${PROJECT_SOURCE_DIR} bitmap-cpp/bitmap.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} bitmap-cpp/*.h)

macro(add_deps)
  foreach(HALIDE_TARGET ${HALIDE_TARGETS})
    add_dependencies(${targetName} "${HALIDE_TARGET}.exec_generator")
    target_include_directories(${targetName} PUBLIC
      "${PROJECT_BINARY_DIR}/generator_genfiles")
    install(FILES "${PROJECT_BINARY_DIR}/generator_genfiles/${HALIDE_TARGET}/${HALIDE_TARGET}.a"
      DESTINATION lib)
  endforeach()
endmacro()

function(add_library targetName)
  _add_library(${targetName} ${ARGN})
  add_deps()
endfunction()

function(add_executable targetName)
  _add_executable(${targetName} ${ARGN})
  add_deps()
endfunction()

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

# install(EXPORT BitmapCppTarget
#   DESTINATION cmake)

install(DIRECTORY bitmap-cpp/ DESTINATION include/bitmap-cpp
  FILES_MATCHING PATTERN "*.h")
