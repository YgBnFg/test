cmake_minimum_required(VERSION 2.8.3)
project(test)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL Xml XmlPatterns Script)
find_package(Boost COMPONENTS system filesystem thread date_time iostreams serialization chrono)
find_package(JsonCpp REQUIRED)
find_package(GTest REQUIRED)
option(WITH_UNITTEST "UNITTEST" on)
message(STATUS "GTEST found: ${GTEST_FOUND}")

set(QT_LIBRARIES Qt5::Widgets Qt5::OpenGL Qt5::Xml Qt5::XmlPatterns Qt5::Script)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${BOOST_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(unittest)
add_subdirectory(UnitTestRuner)
