cmake_minimum_required(VERSION 3.10)

project(Automated-Builds-Test)

add_executable(Automated-Builds-Test Automated-Builds-Test.cxx)

cmake_minimum_required(VERSION 3.13)
project(Automated-Builds-Test)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Main.cpp)

add_executable(simple_example ${SOURCE_FILES})