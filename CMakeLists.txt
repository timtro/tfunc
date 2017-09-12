cmake_minimum_required (VERSION 2.8)

project(tfunc)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-6.0)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(c++std "-std=c++17")
set(common "-fno-omit-frame-pointer -pg")
set(warnings "-Wall -Wextra -Werror -pedantic")
set(optimizations_debug "-O2")
set(optimizations_release "-O3 -flto -ffast-math")
set(debug "-g")
set(perf_bench "-fno-omit-frame-pointer")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${c++std} ${common} ${debug} ${warnings} ${optimizations_debug}")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${c++std}")

# for clang++ use -fuse-ld=gold to avoid LD error "File format not recognized"
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    # set(sanitizer "-fsanitize=address")
    # set(sanitizer "-fsanitize=memory")
    # set(sanitizer "-fsanitize=memory -fsanitize-memory-track-origins")
    # set(sanitizer "-fsanitize=thread")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${sanitizer}")
  endif(CMAKE_BUILD_TYPE MATCHES DEBUG)
endif()

find_package(benchmark)

add_subdirectory (bench)

enable_testing()
add_subdirectory (test)
