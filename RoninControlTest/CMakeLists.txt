cmake_minimum_required (VERSION 3.13)
project (Test)
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Test ${SOURCES})

# Adding c++ flag.
set_target_properties(Test PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS  FALSE
)

target_link_libraries(Test pthread)
