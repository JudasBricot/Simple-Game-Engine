#include "jepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Judas_Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		JE_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		JE_CORE_ASSERT(status, "Failed to initialize Glad!");

		const char* vendor = (const char*)glGetString(GL_VENDOR);
		const char* renderer = (const char*)glGetString(GL_RENDERER);
		const char* version = (const char*)glGetString(GL_VERSION);
		
		JE_CORE_INFO("OpenGL Infos:");
		JE_CORE_INFO("	- Vendor:   {0}", vendor);
		JE_CORE_INFO("	- Renderer: {0}", renderer);
		JE_CORE_INFO("	- Version:  {0}", version);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}