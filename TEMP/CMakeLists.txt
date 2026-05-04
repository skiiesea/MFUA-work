cmake_minimum_required(VERSION 3.20)
project(HelloWorld LANGUAGES CXX)

if (NOT MSVC)
    message(FATAL_ERROR "CLR PROJECT REQUIRES MSVC COMPILER!!!!!")
endif()

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

add_executable(HelloWorld WIN32
    main.cpp
    HelloForm.h
    HelloForm.cpp
)

set_target_properties(HelloWorld PROPERTIES
    COMMON_LANGUAGE_RUNTIME ""
    VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2"
    VS_DOTNET_REFERENCES "System;System.ComponentModel;System.Data;System.Collections;System.Windows.Forms;System.Drawing"
)