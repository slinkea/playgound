cmake_minimum_required (VERSION 3.12)

project(Playground VERSION 1.0 LANGUAGES CXX)

include_directories(components/include)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	link_directories(components/lib/vc16/debug)
else()   
	link_directories(components/lib/vc16/release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /DUNICODE")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")

enable_testing()
add_executable(Playground "src/HDF5Test.h" "src/HDF5Test.cpp")
target_link_libraries(Playground debug libhdf5_D debug gtestd debug gtest_maind 
optimized gtest optimized gtest_main optimized libhdf5)

add_test(Playground Playground)
