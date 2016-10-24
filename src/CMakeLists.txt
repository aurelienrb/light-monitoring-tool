project(light-monitor) # micromon underload microchecker microspy
cmake_minimum_required(VERSION 2.8.11)

#set(CMAKE_INCLUDE_PATH ${CONAN_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})

#include(FindBoost)
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#find_package(Boost 1.60 COMPONENTS system filesystem REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	message(STATUS "### DEBUG mode ###")
	include(${CMAKE_BINARY_DIR}/conanbuildinfo-debug.cmake)
else()
	message(STATUS "### RELEASE mode ###")
	include(${CMAKE_BINARY_DIR}/conanbuildinfo-release.cmake)
endif()

conan_basic_setup()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

include_directories(${CMAKE_SOURCE_DIR}/../libs/Simple-Web-Server)
include_directories(${CMAKE_SOURCE_DIR}/html-files/cpp-generated)

set(GENERATED_CPP_FILES
	html-files/cpp-generated/staticfiles.h
	html-files/cpp-generated/staticfiles.cpp
)

set(SOURCES
	main.cpp
	cpu-stats.h
	cpu-stats.cpp
	monitoring-service.h
	monitoring-service.cpp
	PeriodicTicker.h
	PeriodicTicker.cpp
	CPUMonitor.h
	CPUMonitor.cpp
	HttpServer.h
	HttpServer.cpp
)

add_executable(light-monitor ${SOURCES} ${GENERATED_CPP_FILES})
target_link_libraries(light-monitor ${CONAN_LIBS})
