cmake_minimum_required(VERSION 3.23)
project(rolling_hash VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)
if (!Boost_FOUND)
	message(FATAL_ERROR "Cannot find Boost")
endif ()

set(SOURCES
	src/signature_builder.cpp
	src/serializable_types.cpp
	src/hash.cpp
	src/delta_builder.cpp)

add_executable(rolling_hash src/main.cpp ${SOURCES})
target_include_directories(rolling_hash PUBLIC "${PROJECT_SOURCE_DIR}/src" ${Boost_INCLUDE_DIRS})

add_subdirectory(test)
