#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>

#include <imgui/imgui.h>

#define B_SIZE 256
#define B_LOG_2_SIZE 8

class ButterflyTextureLayer : public Judas_Engine::Layer
{
public:
	ButterflyTextureLayer()
		: Layer("Butterfly Texture Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Plane);
		m_Mesh->SetShader(m_Shader);

		m_ButterflyRenderTexture = Judas_Engine::RenderTexture2D::Create(B_SIZE, B_LOG_2_SIZE);
		m_ButterflyTextureComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/butterflyTexture.glsl");

		m_ButterflyRenderTexture->Bind(0);
		m_ButterflyTextureComputeShader->Dispatch(B_SIZE / 16, B_LOG_2_SIZE, 1);
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_ButterflyRenderTexture;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyTextureComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};
