#pragma once

#include "glad.h"
#include <iostream>
#include <csignal>
#include <string>

#define SIGTRAP 5 // redefined for use of windows compilation
#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const std::string function, const std::string file, int line);