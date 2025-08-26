#pragma once

//#include "renderer.h"
#include "vertexBuffer.h"
//#include "vertexBufferLayout.h"

class VertexBufferLayout;

class VertexArray {

private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

    inline unsigned int getID() const {
        return m_RendererID;
    }
};