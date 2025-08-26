#pragma once
//#include <GLFW/glfw3.h>
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "shader.h"

class Renderer {

public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};
