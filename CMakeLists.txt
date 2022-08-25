cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
include(FetchContent)

# HUNTER_URL is the URL to the latest source code archive on GitHub
# HUNTER_SHA1 is the hash of the downloaded archive

set(HUNTER_URL "https://github.com/cero-lang/hunter/archive/refs/heads/llvm-updates.zip")
set(HUNTER_SHA1 "7319B54F9C9A4C151AE2A901DD9DD7A821DF32EF")
set(HUNTER_PACKAGES LLVM)
set(HUNTER_LLVM_VERSION 14.0.6)
set(HUNTER_LLVM_CMAKE_ARGS
  LLVM_ENABLE_CRASH_OVERRIDES=OFF
  LLVM_ENABLE_ASSERTIONS=ON
  LLVM_ENABLE_PROJECTS=lld
)

FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

project(Cero LANGUAGES CXX)
find_package(LLVM REQUIRED CONFIG)
separate_arguments(LLVM_DEFINITIONS)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM libraries: ${LLVM_LIBRARIES}")
message(STATUS "LLVM includes: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")
message(STATUS "LLVM tools: ${LLVM_TOOLS_BINARY_DIR}")

add_library(LLVM INTERFACE)
target_include_directories(LLVM SYSTEM INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_libraries(LLVM INTERFACE ${LLVM_AVAILABLE_LIBS})
target_compile_definitions(LLVM INTERFACE ${LLVM_DEFINITIONS} -DNOMINMAX)

# MSVC is very particular about not linking version libraries for debugging builds.
if(MSVC)
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

add_subdirectory(src/cero-bootstrap)
