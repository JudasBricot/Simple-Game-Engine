#include "Window.h"

#include <iostream>
 
Window::Window(WindowProp props)
    : m_Props(props)
{
    if(!Init())  
    {
        std::cout << "Failed to create GLFW context." << std::endl;
        return;
    }

    if(!CreateWindow())
    {
        std::cout << "Failed to create GLFW context." << std::endl;
        return;
    }

    std::cout << "Created GLFW context and window successfully" << std::endl;

    glfwMakeContextCurrent(m_Window);
}

Window::~Window()
{
    Shutdown();
    glfwTerminate();
}

Window* Window::Create(WindowProp props)
{
    return new Window(props);
}

void Window::Update()
{
    OnUpdate();
}

bool Window::Init() const
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

void Window::Shutdown() const
{
    glfwDestroyWindow(m_Window);
}

bool Window::CreateWindow()
{
    m_Window = glfwCreateWindow(m_Props.width, m_Props.height, m_Props.name, NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        return false;
    }

    return true;
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::UseVSync(bool value)
{
    if (value)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    std::cout << "UseVsync : value" << std::endl;
}
