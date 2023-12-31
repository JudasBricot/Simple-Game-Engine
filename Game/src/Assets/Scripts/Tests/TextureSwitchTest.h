#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include "Assets/Scripts/Tests/PhillipsTest.h"


class TextureSwitchLayer : public Judas_Engine::Layer
{
public:
	TextureSwitchLayer()
		: Layer("My Texture switch Layer") {}

	virtual void OnAttach()
	{
		// SETUP WIND PARAMETERS
		m_Data = std::make_shared<Ph_Data>(
			1.0f,
			glm::vec2(1.0f, 0.0f),
			31.0f,
			9.8f,
			0.1f,
			glm::vec2(0.5f)
		);
		m_Data->Time = 65412.0f;

		m_Data1 = std::make_shared<Ph_Data>(
			1.0f,
			glm::vec2(1.0f, 0.0f),
			31.0f,
			9.8f,
			0.1f,
			glm::vec2(0.5f)
		);
		m_Data1->Time = 612.0f;

		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_RenderTexture = Judas_Engine::RenderTexture2D::Create(256, 256);
		m_ComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl", m_RenderTexture, 0);

		m_DataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_Data, sizeof(Ph_Data));
		m_DataBuffer1 = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_Data1, sizeof(Ph_Data));

		m_RenderTexture->Bind(0);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::DragInt("Use Texture", &textureNb, 0.01f, 0, 1);
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		if (textureNb)
		{
			m_DataBuffer->Bind(1);
			m_DataBuffer->UpdateData((Judas_Engine::Ref<void>)m_Data);
		}
		else
		{
			m_DataBuffer1->Bind(1);
			m_DataBuffer1->UpdateData((Judas_Engine::Ref<void>)m_Data1);
		}

		m_ComputeShader->Dispatch(SIZE / 16, SIZE / 16, 1);
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_RenderTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_DataBuffer;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_DataBuffer1;

	Judas_Engine::Ref<Ph_Data> m_Data;
	Judas_Engine::Ref<Ph_Data> m_Data1;
	float time = 0.0f;
	int textureNb = 0;
};
