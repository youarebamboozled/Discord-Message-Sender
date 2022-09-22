cmake_minimum_required(VERSION 3.23)
project(src)

set(CMAKE_CXX_STANDARD 23)

add_executable(src main.cpp)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 871ed52d350214a034f6ef8a3b8f51c5ce1bd400)
FetchContent_MakeAvailable(cpr)

target_link_libraries(src PRIVATE cpr::cpr)
