cmake_minimum_required (VERSION 3.8)

project(Playground VERSION 1.0 LANGUAGES CXX CUDA)

find_package(CUDA 11.1 REQUIRED)

include_directories(components/include)
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	link_directories(components/lib/vc16/debug)
else()   
	link_directories(components/lib/vc16/release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /DUNICODE")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG")
set(CMAKE_CUDA_ARCHITECTURES 52)

add_compile_definitions(ZMQ_STATIC)

file(GLOB
  _Src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cu
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cuh
)

add_executable(Playground ${_Src})

#set_target_properties(Playground PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(Playground PROPERTIES LINK_FLAGS "/ignore:4099")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_link_libraries(Playground libhdf5_D log4cplusSUD cnpy 
  uSockets uv_a userenv  
  opencv_core451d ippicvmt ippiwd ittnotifyd zlibd 
  libzmq-v142-mt-sgd-4_3_4 ws2_32 wsock32 Iphlpapi)
else()   
	target_link_libraries(Playground libhdf5 log4cplusSU cnpy 
	uSockets uv_a iphlpapi userenv psapi 
	opencv_core451 ippicvmt ippiw ittnotify zlib  
	libzmq-v142-mt-s-4_3_4 ws2_32 wsock32 Iphlpapi)
endif()

enable_testing()
add_executable(PlaygroundTest "src/tests/PlaygroundTest.h" "src/tests/PlaygroundTest.cpp")
set_target_properties(PlaygroundTest PROPERTIES LINK_FLAGS "/ignore:4099")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_link_libraries(PlaygroundTest 
  gtestd gtest_maind)
else()   
	target_link_libraries(PlaygroundTest 
	gtest gtest_main)
endif()

add_test(PlaygroundTest PlaygroundTest)
