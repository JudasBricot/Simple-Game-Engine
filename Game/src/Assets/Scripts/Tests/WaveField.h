#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include "Assets/Scripts/Ocean/IFFT2D.h"

#define SIZE 256
#define LOG_2_SIZE 8

struct FFT_Data
{
	int stage = 0;
	int pingpong = 0;
	int direction = 0;
};

struct Inversion_Data
{
	int pingpong = 0;
};

class FFTLayer : public Judas_Engine::Layer
{
public:
	FFTLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		// -------------------
		// SPECTRUM GENERATION
		// -------------------

		// SETUP SPECTRUM PARAMETERS
		m_SpectrumData = std::make_shared<Ph_Data>(
			1.0f,
			glm::vec2(1.0f, 0.0f),
			31.0f,
			9.8f,
			0.1f,
			glm::vec2(1.0f)
		);
		m_SpectrumData->Time = 46832.654f;

		// TO CENTER 0,0
		m_SpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);
		m_SpectrumComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl", m_SpectrumTexture, 0);

		m_SpectrumDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_SpectrumData, sizeof(Ph_Data));

		m_IFFT2D = std::make_shared<IFFT2D<256, 8>>(m_SpectrumTexture);
		m_WaveTexture = m_IFFT2D->GetOutputTexture();
	}

	void updateSpectrum()
	{
		m_SpectrumTexture->Bind(0);
		m_SpectrumDataBuffer->Bind(1);
		m_SpectrumDataBuffer->UpdateData((Judas_Engine::Ref<void>)m_SpectrumData);

		m_SpectrumComputeShader->Dispatch(SIZE / 16 + 1, SIZE / 16 + 1, 1);

		m_SpectrumTexture->Unbind();
		m_SpectrumDataBuffer->Unbind();
	}

	void updateWave()
	{
		updateSpectrum();
		m_IFFT2D->Compute();
		/*fft(); */
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		/*if (ImGui::Button("FFT_Stage"))
		{
			fft_stage(stage, pingpong, direction);
			direction = stage == LOG_2_SIZE - 1 ? 0 : direction;
			stage = (stage + 1) % LOG_2_SIZE;
			pingpong = (pingpong + 1) % 2;
		}

		if (ImGui::Button("Permutation"))
		{
			displacement();
		}

		ImGui::InputInt("Stage", &stage, 0, LOG_2_SIZE);
		ImGui::InputInt("pingpong", &pingpong, 0, 1);
		ImGui::InputInt("direction", &direction, 0, 1);*/

		ImGui::InputInt("Render Mode", &m_RenderMode, 0, 3);
		if (m_RenderMode == 1)
		{
			ImGui::DragFloat2("Step", glm::value_ptr(m_SpectrumData->Step), 0.001f, 0.0f, 5.0f);
			ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_SpectrumData->Wind_direction), 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Amplitude", &m_SpectrumData->Amplitude, 0.001f, 0.1f, 2.0f);
			ImGui::DragFloat("Wind Amplitude", &m_SpectrumData->Wind_amplitude, 0.01f, 0.0f, 60.0f);
			ImGui::DragFloat("Gravity", &m_SpectrumData->Gravity, 0.001f, 0.0f, 1.0f);
			ImGui::DragFloat("Min Wave Height Sqrd", &m_SpectrumData->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
			m_SpectrumData->UpdateData();
		}

		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_SpectrumData->Time += ts;
		updateWave();

		/*switch (m_RenderMode)
		{
		case 0:
			m_ButterflyRenderTexture->Bind(0);
			break;
		case 1:
			m_SpectrumTexture->Bind(0);
			break;
		case 2:
			if(m_DisplacementRenderTexture != nullptr)
				m_DisplacementRenderTexture->Bind(0);
			break;
		case 3:
			if (pingpong == 1)
				m_WaveTexture->Bind(0);
			else
				m_SpectrumTexture->Bind(0);
		default:
			break;
		}*/

		m_WaveTexture->Bind(0);

		m_Mesh->Submit();

		m_WaveTexture->Unbind();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumTexture;
	/*Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_WaveTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_ButterflyRenderTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementRenderTexture;*/

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_SpectrumComputeShader;
	/*Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyTextureComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyPassComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_InversionPassComputeShader;*/

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_SpectrumDataBuffer;
	/*Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_ButterflyDataBuffer;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_FFTDataBuffer;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_InversionDataBuffer;*/

	Judas_Engine::Ref<Ph_Data> m_SpectrumData;
	/*Judas_Engine::Ref<Butterfly_Data> m_ButterflyData;
	Judas_Engine::Ref<FFT_Data> m_FFTData;
	Judas_Engine::Ref<Inversion_Data> m_Inversion_Data;*/

	Judas_Engine::Ref<IFFT2D<256,8>> m_IFFT2D;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_WaveTexture;

	int m_RenderMode = 0;
	float time = 0.0f;

	//int stage = 0, direction = 1, pingpong = 0;
};
