#include "Texture.h"

#include "Renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "GL/glew.h"
#include <iostream>

Texture::Texture(std::string filePath)
    : m_FilePath(filePath), m_Width(0), m_Height(0), m_ChannelNb(0)
{
    stbi_set_flip_vertically_on_load(1);

    m_LocalBuffer = stbi_load("res/Textures/grass.png", &m_Width, &m_Height, &m_ChannelNb, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (!m_LocalBuffer)
    {
        std::cout << "Failed to load texture of file path" << filePath << std::endl;
        ASSERT(false);
    }

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
