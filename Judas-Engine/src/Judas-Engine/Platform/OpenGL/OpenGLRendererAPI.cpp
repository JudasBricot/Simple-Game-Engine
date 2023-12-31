#include "jepch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::SetViewPort(int x, int y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRendererAPI::SetDrawMode(DrawMode drawMode)
	{
		switch (drawMode)
		{
		case Judas_Engine::RendererAPI::WireFrame:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case Judas_Engine::RendererAPI::Triangle:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Judas_Engine::RendererAPI::Point:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		default:
			break;
		}
	}
	// ---- TEMP -----

	void OpenGLRendererAPI::EnableBackFaceCulling()
	{
		glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
		glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
		glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
