#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include <glm/glm.hpp>

class SsboLayer : public Judas_Engine::Layer
{

	struct Data
	{
		glm::vec4 color; // WARNING : aligned as glm::vec4 by OpenGL !!
		float time;
		//float pulsation; // To see if the alignment does not break everything
	};

public:
	SsboLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_RenderTexture = Judas_Engine::RenderTexture2D::Create(16, 16);
		m_ComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/ssbo.glsl");

		m_Data = std::make_shared<Data>();
		m_Data->color = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

		m_Ssbo = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_Data, sizeof(Data));
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::DragFloat3("Color", glm::value_ptr(m_Data->color), 0.001f, 0.0f, 1.0f);
		//ImGui::DragFloat("Pulsation", &m_Data->pulsation, 0.001f, 0.0f, 100.0f);
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLRenderTexture2D>(m_RenderTexture)->Bind(0);

		m_Ssbo->Bind(1);
		m_Ssbo->UpdateData((Judas_Engine::Ref<void>)m_Data);
		m_ComputeShader->Dispatch(1, 1, 1);

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref<Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_RenderTexture;
	Judas_Engine::Ref<Judas_Engine::ComputeShader> m_ComputeShader;

	Judas_Engine::Ref<Data> m_Data;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_Ssbo;
};
