#include "shader.h"
#include "commonInclude.h"
#include "vendor/glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <sstream>



Shader::Shader(const std::string& filepath) 
    : m_Filepath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCALL(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        //std::cout << "name: " << m_UniformLocationCache[name] << std::endl;
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " does not exist" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::setUniform1i(const std::string &name, int value)
{
    GLCALL(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string &name, float value)
{
    GLCALL(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &mat)
{
    GLCALL(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat)));
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram(); // for some reason no params
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader); // these should be read from files

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // most of these function names are self explanitory
    glDeleteShader(vs);
    glDeleteShader(fs);
    // deleting intermediate shaders now that they've been attactched to the main program

    // TODO: ALSO ERROR HANDLING

    return program;
}

unsigned int Shader::compileShader(unsigned int type,const std::string& source) {
    // things like GL_VERTEX_SHADER are really just fancy uints
    unsigned int id = glCreateShader(type); // should take in type
    const char * src = source.c_str(); // c_str() returns the "c-style" version of the string
    glShaderSource(id, 1, &src, nullptr); // nullptr is taking up the length param, which indicates string 
    // is nullterminated (that's why we're using a c-style string)
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    // where GL_COMPILE_STATUS is defined, you can insert any kind of query to the state of your program
    // the function writes your error status into result
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char * message = (char*)alloca(length * sizeof(char)); // I think char message[length]; would have worked on my system
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << 
        "shader" << std::endl; // whoa!!! an error message that actually was readable (he would love rust)
        // ternary operator 😍😍😍
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    // TODO: HANDLE ERRORS

    return id;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath) {
    // this is a C++ way to do this, I could search
    // up the C way to do it
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while(getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
            // set mode to vertex
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
    // no need to make a full struct

}