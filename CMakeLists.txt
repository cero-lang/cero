cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(Cero LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.1.0
)

FetchContent_Declare(magic_enum
    GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
    GIT_TAG "v0.8.1"
)

FetchContent_Declare(GSL
  GIT_REPOSITORY https://github.com/microsoft/GSL.git
  GIT_TAG        v4.0.0
)

FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(magic_enum)
FetchContent_MakeAvailable(GSL)

add_subdirectory(cero)
