#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>

#include <imgui/imgui.h>

class ComputeShaderLayer : public Judas_Engine::Layer
{
public:
	ComputeShaderLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_RenderTexture = Judas_Engine::RenderTexture2D::Create(1024, 1024);
		m_ComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/compute.glsl", m_RenderTexture);

		std::dynamic_pointer_cast<Judas_Engine::OpenGLComputeShader>(m_ComputeShader)->Dispatch();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Coucou");
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLRenderTexture2D>(m_RenderTexture)->Bind(15);
		
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_RenderTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};
