cmake_minimum_required (VERSION 3.12)

project(Playground VERSION 1.0 LANGUAGES CXX)

include_directories(components/include)
include_directories(components/include/asio/include)

add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN10)
add_definitions(-DASIO_STANDALONE -DASIO_SEPARATE_COMPILATION)
add_definitions(-D_WEBSOCKETPP_CPP11_TYPE_TRAITS_ -D_WEBSOCKETPP_CPP11_INTERNAL_)


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	link_directories(components/lib/vc16/debug)
else()   
	link_directories(components/lib/vc16/release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /W3 /GR /EHsc /DUNICODE")
set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")
set(CMAKE_CXX_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG"	)

enable_testing()
add_executable(Playground 
	"src/HDF5CscanTest.h" "src/HDF5CscanTest.cpp"
	"src/HDF5AscanTest.h" "src/HDF5AscanTest.cpp"
	"src/HDF5Test.h" "src/HDF5Test.cpp"
	"src/WebSocketTest.h" "src/WebSocketTest.cpp"
	"src/WebSocketClient.h" "src/WebSocketClient.cpp"
	"src/WebSocketServer.h" "src/WebSocketServer.cpp")
target_link_libraries(Playground debug libhdf5_D debug gtestd debug gtest_maind debug zlibstaticd debug libhdf5_hl_D
optimized gtest optimized gtest_main optimized libhdf5 optimized zlibstatic optimized libhdf5_hl uSockets)

add_test(Playground Playground)
