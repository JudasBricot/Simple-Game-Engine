#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>

#include <imgui/imgui.h>

#define B_SIZE 256
#define B_LOG_2_SIZE 8

struct Butterfly_Data
{
	int reverse_bits(int n) 
	{
		int k = 0;
		for (size_t i = 0; i < m_BitNumber; i++)
		{
			k += (1 << (m_BitNumber - i - 1)) * ((n & (1 << i)) >> i);
		}

		return k;
	}

	int BitReverseTable[B_SIZE];

	Butterfly_Data(int bitNumber) : m_BitNumber(bitNumber)
	{
		for (int i = 0; i < B_SIZE; i++)
		{
			BitReverseTable[i] = reverse_bits(i);
		}
	}

	const int m_BitNumber;
};

class ButterflyLayer : public Judas_Engine::Layer
{
public:
	ButterflyLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_ButterflyData = std::make_shared<Butterfly_Data>(B_LOG_2_SIZE);
		m_ButterflyDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_ButterflyData, sizeof(Butterfly_Data));

		m_ButterflyRenderTexture = Judas_Engine::RenderTexture2D::Create(B_SIZE, B_LOG_2_SIZE);
		m_ButterflyTextureComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/butterflyTexture.glsl");

		m_ButterflyRenderTexture->Bind(0);
		m_ButterflyDataBuffer->Bind(1);

		m_ButterflyTextureComputeShader->Dispatch(B_SIZE / 16, B_LOG_2_SIZE, 1);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Coucou");
		ImGui::End();
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

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_ButterflyDataBuffer;
	Judas_Engine::Ref<Butterfly_Data> m_ButterflyData;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};
