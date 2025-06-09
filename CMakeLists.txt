cmake_minimum_required(VERSION 3.10)

project(AOC_19_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(raylib REQUIRED)


file(GLOB_RECURSE SOURCES
    main.cpp
    day*/*.cpp
    intcode/*.cpp
)

file(GLOB_RECURSE HEADERS
    day*/·h
    day*/.hpp
    intcode/*.h
    intcode/*.hpp
    util/*.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/day1
    ${CMAKE_SOURCE_DIR}/day13
    ${CMAKE_SOURCE_DIR}/intcode
    ${CMAKE_SOURCE_DIR}/util
)

add_executable(AOC_19_CPP ${SOURCES} ${HEADERS})
target_link_libraries(AOC_19_CPP raylib)

