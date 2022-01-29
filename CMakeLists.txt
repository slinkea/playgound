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
	"src/WebSocketTest.h" "src/WebSocketTest.cpp"
	"src/WebSocketClient.h" "src/WebSocketClient.cpp"
	"src/WebSocketServer.h" "src/WebSocketServer.cpp"
	"src/WebSocketWorker.h" "src/WebSocketWorker.cpp"
	"src/Event.hpp" "src/EventArguments.h")

target_link_libraries(Playground debug gtestd debug gtest_maind debug zlibstaticd debug log4cplusSUD
optimized gtest optimized gtest_main optimized zlibstatic optimized log4cplusSU
uSockets)

add_test(Playground Playground)
