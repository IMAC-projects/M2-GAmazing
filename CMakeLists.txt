CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project(GAMandelbrot)

set(EXECUTABLE_NAME GAMandelbrot)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Add source files
file(GLOB_RECURSE SRC_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
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

#SDL2
message("Looking for SDL2...")
find_package(SDL2 REQUIRED)
if(${SDL2_FOUND})
    message(STATUS "lib SDL2 found")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g++")

INCLUDE_DIRECTORIES(lib lib/glm ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

set(ALL_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} glfw ${GLFW_LIBRARIES})

ADD_EXECUTABLE(${EXECUTABLE_NAME} ${SRC_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${ALL_LIBRARIES})