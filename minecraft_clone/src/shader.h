#pragma once 
#include <string>
#include <unordered_map>

#include "vendor/glm/glm.hpp"
//#include "vendor/glm/gtc/matrix_transform.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filename);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // set uniforms
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, const glm::mat4& mat);
private:
    ShaderProgramSource parseShader(const std::string& filepath);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int compileShader(unsigned int type,const std::string& source);
    int getUniformLocation(const std::string& name);

    
};