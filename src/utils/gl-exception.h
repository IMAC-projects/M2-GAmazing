#pragma once

#include <GL/glew.h>
#include <cstdio>
#include <assert.h>
/**
 * @brief Assertion and logger handling for opengl functions
 */
#ifndef NDEBUG
    #define GLCall(x) glexp::clear(); x;
#else
    #define GLCall(x) x
#endif

namespace glexp {
    /**
     * @brief Empty the OpenGl error buffer
     */
    void clear();

    /**
     * @brief Print OpenGl errors to the console if any
     * 
     * @param functionName
     * @param filename
     * @param line
     */
    bool doesFunctionWorks(const char* functionName, const char* filename, int line);

    /**
     * @brief Transform error enum to text for OpenGL
     * 
     * @param err 
     * @return char const* - Text corresponding to OpenGl error
     */
    char const* glErrorString(GLenum const err);
}