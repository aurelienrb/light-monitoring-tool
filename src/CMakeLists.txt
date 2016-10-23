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

include_directories(${CMAKE_SOURCE_DIR}/../libs/Simple-Web-Server)
include_directories(${CMAKE_SOURCE_DIR}/html-files/cpp-generated)

set(GENERATED_CPP_FILES
	html-files/cpp-generated/staticfiles.h
	html-files/cpp-generated/staticfiles.cpp
)

set(SOURCES
	main.cpp
)

add_executable(light-monitor ${SOURCES} ${GENERATED_CPP_FILES})
target_link_libraries(light-monitor ${CONAN_LIBS})
