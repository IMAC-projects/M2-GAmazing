cmake_minimum_required(VERSION 3.5)

project(GAmazing)

set(EXECUTABLE_NAME GAmazing)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/")

# Add source files
file(GLOB_RECURSE SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/utils/*.cpp
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/utils/*.h
    ${CMAKE_SOURCE_DIR}/src/utils/*.hpp
	${CMAKE_SOURCE_DIR}/src/*.hpp)

## INCLUDE LIB ##

#OpenGL
message("Looking for OpenGL...")
FIND_PACKAGE(OpenGL REQUIRED)
if(${OpenGL_FOUND})
    message(STATUS "lib OpenGL found")
endif()

#GLEW
message("Looking for GLEW...")
FIND_PACKAGE(GLEW REQUIRED)
if(${GLEW_FOUND})
    message(STATUS "lib GLEW found")
endif()

# Eigen
find_package(Eigen3 REQUIRED)
if(${EIGEN3_FOUND})
    message(STATUS "lib EIGEN3 found")
    message(STATUS "  version " ${EIGEN3_VERSION_STRING})
    message(STATUS "  include " ${EIGEN3_INCLUDE_DIR})
endif()

#SDL2
message("Looking for SDL2...")
find_package(SDL2 REQUIRED)
if(${SDL2_FOUND})
    message(STATUS "lib SDL2 found")
endif()


# require lib c3ga
find_package(C3GA)
if(${C3GA_FOUND})
    message(STATUS "lib C3GA found")
    message(STATUS "  include " ${C3GA_INCLUDE_DIRS})
    message(STATUS "  lib     " ${C3GA_LIBRARIES})
    else()
    message(STATUS "lib C3GA should be set manually, please uncomment the commented lines in the CMakeLists.txt file")

    # linux users
    set(C3GA_INCLUDE_DIRS "/home/laurine/Documents/MASTER/GA/garamon_c3ga/src/")
    set(C3GA_LIBRARIES "/home/laurine/Documents/MASTER/GA/garamon_c3ga/build/libc3ga.so")

    # for manual setting, check if valid
    if(EXISTS "${C3GA_INCLUDE_DIRS}")
        message(STATUS "include for lib c3ga found: ${C3GA_INCLUDE_DIRS}" )
    else()
        message(FATAL_ERROR "include for lib c3ga invalid or not found: ${C3GA_INCLUDE_DIRS}" )
    endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++14")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g++")

INCLUDE_DIRECTORIES(lib lib/glm ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR} ${C3GA_INCLUDE_DIRS})

set(ALL_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${GLFW_LIBRARIES})

ADD_EXECUTABLE(${EXECUTABLE_NAME} ${SRC_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${ALL_LIBRARIES})