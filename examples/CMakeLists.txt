cmake_minimum_required(VERSION 3.11)
project(aimc-sim LANGUAGES CXX)

# CMake includes
include(FetchContent)

# CXX settings
set(CMAKE_CXX_STANDARD 17 CACHE STRING "v")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Tooling support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(s1_src scenario_1.cpp)

# Fetch libraries/packages

# SystemC
set(ENABLE_EXAMPLES OFF CACHE INTERNAL "Turn off SystemC tests")
set(ALLOW_DEPRECATED_IEEE_API ON CACHE INTERNAL "Suppress warnings for deprecated API use")
FetchContent_Declare(
  systemc
  GIT_REPOSITORY https://github.com/accellera-official/systemc.git
  GIT_TAG 3.0.0
)
message(STATUS "Fetching SystemC...")
FetchContent_MakeAvailable(systemc)

# SystemC CCI
set(SYSTEMCCCI_BUILD_TESTS OFF CACHE INTERNAL "Turn off SystemC CCI tests")
FetchContent_Declare(
  cci
  GIT_REPOSITORY https://github.com/accellera-official/cci.git
  GIT_TAG 2f70602c8753220e02778fcab040f197c0747f84
)
message(STATUS "Fetching SystemC CCI...")
FetchContent_MakeAvailable(cci)

add_executable(scenario_1_ex ${s1_src})

target_link_libraries(scenario_1_ex PUBLIC systemc cci)
