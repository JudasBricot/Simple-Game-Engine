#include "Renderer.h"

#include <GL/glew.h>
#include <iostream>

void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpengGL Error] (" << error << "):" << function << " " << file << " " << line << std::endl;
        return false;
    }

    return true;
}
