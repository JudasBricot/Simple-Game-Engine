#pragma once

#include <JudasEngine.h>

#include <imgui/imgui.h>

class PrimitiveTestLayer : public Judas_Engine::Layer
{
public:
	PrimitiveTestLayer()
		: Layer("MyLayer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/colors.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(m_PrimitiveType);
		m_Mesh->SetShader(m_Shader);
	}

	void SetPrimitiveType(const Judas_Engine::PrimitiveType& primitiveType)
	{
		if (primitiveType == m_PrimitiveType)
			return;

		m_PrimitiveType = primitiveType;
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(m_PrimitiveType, 16);
		m_Mesh->SetShader(m_Shader);

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Cube", ImVec2()))
		{
			SetPrimitiveType(Judas_Engine::PrimitiveType::Cube);
		}
		if (ImGui::Button("Plane", ImVec2()))
		{
			SetPrimitiveType(Judas_Engine::PrimitiveType::Plane);
		}
		if (ImGui::Button("Sphere", ImVec2()))
		{
			SetPrimitiveType(Judas_Engine::PrimitiveType::Sphere);
		}
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::PrimitiveType m_PrimitiveType = Judas_Engine::PrimitiveType::Cube;
};
