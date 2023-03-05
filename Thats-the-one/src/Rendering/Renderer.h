#pragma once

#ifdef _DEBUG
    #define ASSERT(x) if(!(x)) __debugbreak();
    #define GLCall(x) GLClearErrors();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x;
#endif // _DEBUG

void GLClearErrors();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Run();
};

