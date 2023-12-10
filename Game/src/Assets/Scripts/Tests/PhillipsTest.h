#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>

#include <imgui/imgui.h>

class PhillipsLayer : public Judas_Engine::Layer
{
public:
	PhillipsLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		// SETUP WIND PARAMETERS
		amplitude = 1.0;
		wind_direction = glm::vec2(1.0f, 0.0f);
		wind_amplitude = 31.0f;

		gravity = 9.8;

		max_wave_height_sqrd = wind_amplitude * wind_amplitude / gravity;
		max_wave_height_sqrd *= max_wave_height_sqrd;
		min_wave_height_sqrd = 0.1 * 0.1;

		step = glm::vec2(0.5f, 0.5f);

		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_RenderTexture = Judas_Engine::RenderTexture2D::Create(256, 256);
		m_ComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl", m_RenderTexture);

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
		time += ts.GetSeconds();

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLRenderTexture2D>(m_RenderTexture)->Bind(0);


		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_RenderTexture;

	float amplitude;
	glm::vec2 wind_direction;
	float wind_amplitude;

	float gravity;

	float max_wave_height_sqrd;
	float min_wave_height_sqrd;

	glm::vec2 step;
	float time = 0.0f;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};