#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "commonInclude.h"
#include <iostream>

VertexArray::VertexArray() {
    GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
    GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}   

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {

        const auto& element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));
        GLCALL(glVertexAttribPointer(i, element.count, element.type, 
            element.normalized, layout.getStride(), reinterpret_cast<const void *>(offset)));

        offset+= element.count * VertexBufferElement::getSizeOfType(element.type);
        // it gets mad that offset is not really a const void * but it compiles regardless
    }
}

void VertexArray::Bind() const {
    GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
    GLCALL(glBindVertexArray(0));
}