#include "WindowsWindow.h"

#include "Core.h"
#include "EventSystem/ApplicationEvent.h"

#include <iostream>

namespace Judas_Engine
{
    static bool  s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& props)
    {
        return new  WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
        glfwTerminate();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        //JE_CORE_INFO("Creating window %s (%d, %d)", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            //JE_CORE_ASSERT(success, "Could not initialize GLFW!");

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = * (WindowData*) glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizedEvent event(width, height);
                data.EventCallback(event);
            });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void WindowsWindow::SetVSync(bool value)
    {
        if (value)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.VSync = value;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
