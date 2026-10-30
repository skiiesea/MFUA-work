cmake_minimum_required(VERSION 3.16)

project(Variant6)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(MotionGL)

add_executable(variant6
    main.cpp
    numerical_methods.cpp
)

target_link_libraries(variant6 PRIVATE motiongl)