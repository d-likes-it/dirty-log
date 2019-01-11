cmake_minimum_required(VERSION 3.5)
project(dirty-log VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
endif()

set (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set (CMAKE_CXX_FLAGS                "-Wall")
set (CMAKE_CXX_FLAGS_DEBUG          "-g")
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

set(CPP_VERSION 98)
if ("cxx_std_11" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	set(CPP_VERSION 11)
endif()
if ("cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	set(CPP_VERSION 14)
endif()
if ("cxx_std_17" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	set(CPP_VERSION 17)
endif()

configure_file("src/build_options.hpp.in"
	"include/build_options.hpp")

add_executable(sandbox "src/sandbox.cpp")
add_executable(dlog "src/dlog.cpp")


target_include_directories(sandbox PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include") #"3rdparty/typestring")
target_include_directories(dlog PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")


