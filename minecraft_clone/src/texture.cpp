#include "texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : m_RendererID(0), m_Filepath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCALL(glGenTextures(1, &m_RendererID));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // linear algebra 😢

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer)); 
    // if this doesn't work, use GL_RBBA8 (num bits per channel) (only for the first one)
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture()
{
    GLCALL(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCALL(glActiveTexture(GL_TEXTURE0 + slot)); // GL_TEXTURE0 is the first texture slot
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}
