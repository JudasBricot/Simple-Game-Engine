#pragma once

#include <JudasEngine.h>

#include <imgui/imgui.h>

class CameraTestLayer : public Judas_Engine::Layer
{
public:
	CameraTestLayer(Judas_Engine::PerspectiveCamera camera)
		: Layer("MyLayer"), m_Camera(camera) {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/colors.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Cube);
		m_Mesh->SetShader(m_Shader);

		//m_Axis = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::PrimitiveType::Cube);
		//m_Axis->SetShader(m_Shader);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Left"))
			m_Camera.RotateAroundAxis(glm::vec3(0, 1, 0), 90.0);
		if (ImGui::Button("Right"))
			m_Camera.RotateAroundAxis(glm::vec3(0, 1, 0), -90.0);
		if (ImGui::Button("Up"))
			m_Camera.RotateAroundAxis(glm::vec3(0, 0, 1), 90.0);
		if (ImGui::Button("Down"))
			m_Camera.RotateAroundAxis(glm::vec3(0, 0, 1), -90.0);
		if (ImGui::Button("Twist Left"))
			m_Camera.RotateAroundAxis(glm::vec3(1, 0, 0), 90.0);
		if (ImGui::Button("Twist Right"))
			m_Camera.RotateAroundAxis(glm::vec3(1, 0, 0), -90.0);
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref<Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::Ref<Judas_Engine::Mesh> m_Axis;

	Judas_Engine::PerspectiveCamera m_Camera;
};
