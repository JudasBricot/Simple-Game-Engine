#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include "Assets/Scripts/Tests/PhillipsTest.h"

struct Ph_Data
{
	glm::vec2 Step;
	glm::vec2 Wind_direction;
	float Amplitude;
	float Wind_amplitude;

	float Gravity;

	float Max_wave_height_sqrd;
	float Min_wave_height_sqrd;

	float Time;

	Ph_Data(float amplitude, glm::vec2 wind_direction, float wind_amplitude, float gravity, float min_wave_height_sqrd, glm::vec2 step)
		: Amplitude(amplitude), Wind_direction(wind_direction), Wind_amplitude(wind_amplitude), Gravity(gravity), Min_wave_height_sqrd(min_wave_height_sqrd), Step(step)
	{
		Max_wave_height_sqrd = Wind_amplitude * Wind_amplitude / Gravity;
		Max_wave_height_sqrd *= Max_wave_height_sqrd;

		Time = 0.0f;
	}

	void UpdateData()
	{
		Max_wave_height_sqrd = Wind_amplitude * Wind_amplitude / Gravity;
		Max_wave_height_sqrd *= Max_wave_height_sqrd;
	}

	void UpdateTime(float deltaTime)
	{
		Time += deltaTime;
	}
};

class PhillipsLayer : public Judas_Engine::Layer
{
public:
	PhillipsLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach() override
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

		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Plane);
		m_Mesh->SetShader(m_Shader);

		m_SpectrumRenderTexture = Judas_Engine::RenderTexture2D::Create(256, 256);
		m_SlopeRenderTexture = Judas_Engine::RenderTexture2D::Create(256, 256);
		m_ComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl");

		m_Ssbo = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_Data, sizeof(Ph_Data));
	}

	virtual void OnDetach() override
	{
		m_Shader->Unbind();
		m_SpectrumRenderTexture->Unbind();
		m_Ssbo->Unbind();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::InputInt("Render mode", &m_RenderMode, 0, 1);

		ImGui::DragFloat2("Step", glm::value_ptr(m_Data->Step), 0.001f, 0.0f, 5.0f);
		ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_Data->Wind_direction), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Amplitude", &m_Data->Amplitude, 0.001f, 0.1f, 2.0f);
		ImGui::DragFloat("Wind Amplitude", &m_Data->Wind_amplitude, 0.01f, 0.0f, 60.0f);
		ImGui::DragFloat("Gravity", &m_Data->Gravity, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("Min Wave Height Sqrd", &m_Data->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
		m_Data->UpdateData();
		ImGui::End();
	}

	void ComputeSpectrum()
	{
		m_SpectrumRenderTexture->Bind(0);
		m_SlopeRenderTexture->Bind(1);
		m_Ssbo->Bind(2);
		m_Ssbo->UpdateData((Judas_Engine::Ref<void>)m_Data);
		m_ComputeShader->Dispatch(SIZE / 16, SIZE / 16, 1);

		m_SpectrumRenderTexture->Unbind();
		m_SlopeRenderTexture->Unbind();
		m_Ssbo->Unbind();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Data->UpdateTime(ts.GetSeconds());
		ComputeSpectrum();

		switch (m_RenderMode)
		{
		case 0:
			m_SpectrumRenderTexture->Bind(0);
			break;
		case 1:
			m_SlopeRenderTexture->Bind(0);
			break;
		default:
			break;
		}

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumRenderTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeRenderTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_Ssbo;

	Judas_Engine::Ref<Ph_Data> m_Data;

	int m_RenderMode = 0;
	const int SIZE = 256;
};