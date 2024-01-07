#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include "Assets/Scripts/Ocean/IFFT2D.h"
#include "Assets/Scripts/Tests/PhongTest.h"

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
	struct Light
	{
		glm::vec3 Direction;

		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;

		void UpdateUniforms(Judas_Engine::Ref<Judas_Engine::Shader> shader)
		{
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.Direction", Direction);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.AmbientColor", AmbientColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.DiffuseColor", DiffuseColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.SpecularColor", SpecularColor);
		}
	};

	struct Material
	{
		glm::vec3 DiffuseColor;
		float DiffuseReflection;

		glm::vec3 SpecularColor;
		float SpecularReflection;
		float SpecularShininess;

		void UpdateUniforms(Judas_Engine::Ref<Judas_Engine::Shader> shader)
		{
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Material.DiffuseColor", DiffuseColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.DiffuseReflection", DiffuseReflection);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Material.SpecularColor", SpecularColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.SpecularReflection", SpecularReflection);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.SpecularShininess", SpecularShininess);
		}
	};
public:
	FFTLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_WaveShader = Judas_Engine::Shader::Create("src/Assets/shaders/waveShader.glsl");
		m_TextureShader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Plane, 256);
		m_Mesh->SetShader(m_WaveShader);

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
		m_SlopeSpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);
		m_DisplacementSpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);

		m_SpectrumComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl");

		m_SpectrumDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_SpectrumData, sizeof(Ph_Data));

		m_WaveIFFT = std::make_shared<IFFT2D<256, 8>>(m_SpectrumTexture);
		m_WaveTexture = m_WaveIFFT->GetOutputTexture();

		m_SLopeIFFT = std::make_shared<IFFT2D<256, 8>>(m_SlopeSpectrumTexture);
		m_SlopeTexture = m_SLopeIFFT->GetOutputTexture();

		m_DisplacementIFFT = std::make_shared<IFFT2D<256, 8>>(m_DisplacementSpectrumTexture);
		m_DisplacementTexture = m_DisplacementIFFT->GetOutputTexture();
	}

	void updateSpectrum()
	{
		m_SpectrumTexture->BindImage(0);
		m_SlopeSpectrumTexture->BindImage(1);
		m_DisplacementSpectrumTexture->BindImage(2);

		m_SpectrumDataBuffer->Bind(3);
		m_SpectrumDataBuffer->UpdateData((Judas_Engine::Ref<void>)m_SpectrumData);

		m_SpectrumComputeShader->Dispatch(SIZE / 16 + 1, SIZE / 16 + 1, 1);

		m_SpectrumTexture->UnbindImage();
		m_SlopeSpectrumTexture->UnbindImage();
		m_DisplacementSpectrumTexture->UnbindImage();

		m_SpectrumDataBuffer->Unbind();
	}

	void updateWave()
	{
		updateSpectrum();
		m_WaveIFFT->Compute();
		m_SLopeIFFT->Compute();
		m_DisplacementIFFT->Compute();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::InputInt("Render Mode", &m_RenderMode, 0, 2);

		ImGui::DragFloat2("Step", glm::value_ptr(m_SpectrumData->Step), 0.001f, 0.0f, 5.0f);
		ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_SpectrumData->Wind_direction), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Amplitude", &m_SpectrumData->Amplitude, 0.001f, 0.1f, 2.0f);
		ImGui::DragFloat("Wind Amplitude", &m_SpectrumData->Wind_amplitude, 0.01f, 0.0f, 60.0f);
		ImGui::DragFloat("Gravity", &m_SpectrumData->Gravity, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("Min Wave Height Sqrd", &m_SpectrumData->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
		m_SpectrumData->UpdateData();

		ImGui::DragFloat("Displacement factor", &m_DisplacementFactor, 0.001f, 0.0f, 1.0f);

		ImGui::End();

		ImGui::Begin("Light properties");

		ImGui::DragFloat3("Direction", glm::value_ptr(m_Light.Direction), 0.001f, -1.0f, 1.0f);
		ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_Light.AmbientColor));
		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_Light.DiffuseColor));
		ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_Light.SpecularColor));

		ImGui::End();


		ImGui::Begin("Material properties");

		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_Material.DiffuseColor));
		ImGui::DragFloat("Diffuse reflection", &m_Material.DiffuseReflection, 0.001f, 0.0f, 10.0f);
		ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_Material.SpecularColor));
		ImGui::DragFloat("Specular reflection", &m_Material.SpecularReflection, 0.001f, 0.0f, 10.0f);
		ImGui::DragFloat("Specular shininess", &m_Material.SpecularShininess, 0.001f, 0.0f, 2.0f);

		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_SpectrumData->Time += ts;
		updateWave();

		// Dispatching compute shader change the used program !!
		if (m_RenderMode == 0)
		{
			m_WaveShader->Bind();
			m_Mesh->SetShader(m_WaveShader);

			m_Light.UpdateUniforms(m_WaveShader);
			m_Material.UpdateUniforms(m_WaveShader);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_WaveShader)->UploadUniformInt("u_HeightMap", 0);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_WaveShader)->UploadUniformInt("u_NormalMap", 1);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_WaveShader)->UploadUniformInt("u_DisplacementMap", 2);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_WaveShader)->UploadUniformFloat("u_DisplacementFactor", m_DisplacementFactor);

			m_WaveTexture->Bind(0);
			m_SlopeTexture->Bind(1);
			m_DisplacementTexture->Bind(2);
		}
		else
		{
			m_TextureShader->Bind();
			m_Mesh->SetShader(m_TextureShader);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_WaveShader)->UploadUniformInt("u_Texture", 0);
			if (m_RenderMode == 1)
				m_WaveTexture->Bind(0);
			else if (m_RenderMode == 2)
				m_SlopeTexture->Bind(0);
			else if (m_RenderMode == 3)
				m_DisplacementTexture->Bind(0);
		}

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_WaveShader;
	Judas_Engine::Ref<Judas_Engine::Shader> m_TextureShader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_SpectrumComputeShader;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_SpectrumDataBuffer;
	Judas_Engine::Ref<Ph_Data> m_SpectrumData;

	Judas_Engine::Ref<IFFT2D<256,8>> m_WaveIFFT;
	Judas_Engine::Ref<IFFT2D<256,8>> m_SLopeIFFT;
	Judas_Engine::Ref<IFFT2D<256,8>> m_DisplacementIFFT;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeSpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementSpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_WaveTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementTexture;

	int m_RenderMode = 0;
	float m_DisplacementFactor = 0.5f;

	Light m_Light = { glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(20.0f, 29.0f, 45.0f) / 256.0f, glm::vec3(1.0f), glm::vec3(1.0f) };
	Material m_Material = { glm::vec3(35.0f, 35.0f, 135.0f) / 256.0f, 0.8f, glm::vec3(1.0f), 0.2f, 16.0f };
};
