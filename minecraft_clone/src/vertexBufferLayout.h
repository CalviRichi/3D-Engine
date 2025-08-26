#pragma once

#include "commonInclude.h"
#include <vector>
#include <stdexcept>
#include "glad.h"
#include <iostream>

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:
                //std::cout << "float" << std::endl;
                return 4;
                break;
            case GL_UNSIGNED_INT:
                std::cout << "int" << std::endl;
                return 4;
                break;
            case GL_UNSIGNED_BYTE:
                std::cout << "byte" << std::endl;
                return 1;
                break;
            default:
                std::cout << "another" << std::endl;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {

private:

    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:

    VertexBufferLayout() : m_Stride(0) {}
    ~VertexBufferLayout() {}

    template<typename T>
    int Push(unsigned int count)
    {
        std::runtime_error("An error occured");
        return -1;
    }

    inline const std::vector<VertexBufferElement> GetElements() const {
        return m_Elements; // if something breaks, return address of m_Elements
    }
    inline unsigned int getStride() const {
        return m_Stride;
    }
};

template<> inline
int VertexBufferLayout::Push<float>(unsigned int count) {
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    return 1;
}

template<> inline
int VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    return 1;
}

template<> inline
int VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    return 1;
}

