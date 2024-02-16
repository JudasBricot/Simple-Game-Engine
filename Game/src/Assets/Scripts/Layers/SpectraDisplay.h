#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#include "Assets/Scripts/Ocean/SpectrumGenerator.h"

class SpectraDisplayLayer : public Judas_Engine::Layer
{
public:
	SpectraDisplayLayer()
		: Layer("Spectra Displayer Layer") {}

	virtual void OnAttach() override
	{
		m_SpectrumGenerator = std::make_shared<SpectrumGenerator<256>>();

		m_SpectrumTexture = m_SpectrumGenerator->GetSpectrumTexture();
		m_SlopeSpectrumTexture = m_SpectrumGenerator->GetSlopeSpectrumTexture();
		m_DisplacementSpectrumTexture = m_SpectrumGenerator->GetDisplacementSpectrumTexture();

		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Plane);
		m_Mesh->SetShader(m_Shader);
	}

	virtual void OnDetach() override
	{
		m_Shader->Unbind();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::InputInt("Render mode", &m_RenderMode, 0, 1);
		m_SpectrumGenerator->ImGuiRender();

		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->UploadUniformFloat("u_Texture", 0);
		m_SpectrumGenerator->ComputeTimeDependentSpectrum(ts);

		switch (m_RenderMode)
		{
		case 0:
			m_SpectrumTexture->Bind(0);
			break;
		case 1:
			m_SlopeSpectrumTexture->Bind(0);
			break;
		default:
			m_DisplacementSpectrumTexture->Bind(0);
			break;
		}

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
	Judas_Engine::Ref<SpectrumGenerator<256>> m_SpectrumGenerator;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeSpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementSpectrumTexture;

	int m_RenderMode = 0;
	const int SIZE = 256;
};