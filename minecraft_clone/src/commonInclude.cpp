#include "commonInclude.h"

void GLClearError() {
    while (glGetError() != GL_NO_ERROR); // or !(glGetError)
}

bool GLLogCall(const std::string function, const std::string file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << 
        " at line: " << line << "\nIn file: " << file << std::endl;
        return false;
    }
    return true;
}