#include "TestManager.h"

#include <Judas-Engine/Renderer/Renderer.h>

#include "imgui/imgui.h"

void TestManagerLayer::OnImGuiRender()
{
	ImGui::Begin("Test Manager");

	for (auto test : m_TestMap)
	{
		ImGui::Text(test.first.c_str());
		if (ImGui::Button("Select", ImVec2(100.0f, 20.0f)))
		{
			if (m_SelectedTest != nullptr)
				m_SelectedTest->OnDetach();
			m_SelectedTest = test.second;
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
