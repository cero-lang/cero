cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
include(FetchContent)

# HUNTER_URL is the URL to thelatest source code archive on GitHub
# HUNTER_SHA1 is the hash of the downloaded archive

set(HUNTER_URL "https://github.com/LLVMParty/hunter/archive/refs/heads/llvm-updates.zip")
set(HUNTER_SHA1 "B90D6A754811222366550BC0A2A82FF1F878C41F")

set(HUNTER_LLVM_VERSION 13.0.1)
set(HUNTER_LLVM_CMAKE_ARGS
        LLVM_ENABLE_CRASH_OVERRIDES=OFF
        LLVM_ENABLE_ASSERTIONS=ON
        LLVM_ENABLE_PROJECTS=clang;lld
        )
set(HUNTER_PACKAGES LLVM)

include(FetchContent)
message(STATUS "Fetching hunter...")

FetchContent_Declare(SetupHunter
  GIT_REPOSITORY https://github.com/cpp-pm/gate
)

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

FetchContent_MakeAvailable(SetupHunter)
FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(magic_enum)
FetchContent_MakeAvailable(GSL)

project(Cero LANGUAGES CXX)

set(CMAKE_FOLDER_LLVM "${CMAKE_FOLDER}")
if(CMAKE_FOLDER)
    set(CMAKE_FOLDER "${CMAKE_FOLDER}/LLVM")
else()
    set(CMAKE_FOLDER "LLVM")
endif()

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Split the definitions properly (https://weliveindetail.github.io/blog/post/2017/07/17/notes-setup.html)
separate_arguments(LLVM_DEFINITIONS)

# Some diagnostics (https://stackoverflow.com/a/17666004/1806760)
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM includes: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")

add_library(LLVM INTERFACE)
target_include_directories(LLVM SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM INTERFACE ${LLVM_AVAILABLE_LIBS})
target_compile_definitions(LLVM INTERFACE ${LLVM_DEFINITIONS} -DNOMINMAX)

set(CMAKE_FOLDER "${CMAKE_FOLDER_LLVM}")
unset(CMAKE_FOLDER_LLVM)

# MSVC-specific options
if(MSVC)
    # This assumes the installed LLVM was built in Release mode
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/ZI /Od /Ob0 /DNDEBUG" CACHE STRING "" FORCE)

    if(${LLVM_USE_CRT_RELEASE} STREQUAL "MD")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
    elseif(${LLVM_USE_CRT_RELEASE} STREQUAL "MT")
        set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded)
    else()
        message(FATAL_ERROR "Unsupported LLVM_USE_CRT_RELEASE=${LLVM_USE_CRT_RELEASE}")
    endif()
endif()

find_package(Clang REQUIRED)

add_subdirectory(cero)
