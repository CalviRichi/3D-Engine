#include "renderer.h"

void Renderer::clear() const {
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind(); // only this one is completely necessary
    ib.Bind();   

    GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
    // this is assuming that unsigned int is hard coded into the renderer
}