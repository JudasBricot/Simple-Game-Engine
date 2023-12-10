#include "TestManager.h"

#include <Judas-Engine/Renderer/Renderer.h>

#include "imgui/imgui.h"

void TestManagerLayer::OnImGuiRender()
{
	ImGui::Begin("Test Manager");

	for (auto const& [key, val] : m_TestMap)
	{
		ImGui::Text(key.c_str());
		std::string s = "Select " + key;
		bool pressed = ImGui::Button(s.c_str());

		if (pressed)
		{
			JE_INFO("{0} Pressed : {1}", key.c_str(), pressed);
			if (m_SelectedTest != nullptr)
				m_SelectedTest->OnDetach();
			m_SelectedTest = val;
			m_SelectedTest->OnAttach();

			break;
		}
	}

	if (m_SelectedTest != nullptr)
		m_SelectedTest->OnImGuiRender();

	ImGui::End();
}

void TestManagerLayer::OnEvent(Judas_Engine::Event& e)
{
	m_CameraController.OnEvent(e);

	if (m_SelectedTest != nullptr)
		m_SelectedTest->OnEvent(e);
}

void TestManagerLayer::OnUpdate(Judas_Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Judas_Engine::Renderer::BeginScene(m_CameraController.GetCamera());

	Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Judas_Engine::RenderCommand::Clear();

	if (m_SelectedTest != nullptr)
		m_SelectedTest->OnUpdate(ts);

	Judas_Engine::Renderer::EndScene();
}
