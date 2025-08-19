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

# SystemC settings
add_compile_definitions(SC_ENABLE_ASSERTIONS=1)

# Source files
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
  GIT_TAG 5a671c61d8f805bd9cbb00a508e403912f9b112b
)
message(STATUS "Fetching SystemC CCI...")
FetchContent_MakeAvailable(cci)

# Targets
add_executable(scenario_1_ex ${s1_src})

# Link libraries to targets
target_link_libraries(scenario_1_ex PUBLIC systemc cci)
