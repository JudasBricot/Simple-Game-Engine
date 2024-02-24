#include "Sandbox2D.h"

#include "Judas-Engine/Platform/OpenGL/OpenGLShader.h"
#include "Judas-Engine/Renderer/Renderer2D.h"

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

Sandbox2D::Sandbox2D() : Layer("MyLayer"), m_CameraController(1280.0f / 720.0f, true)
{ }

void Sandbox2D::OnAttach()
{
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Judas_Engine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Judas_Engine::RenderCommand::Clear();

	Judas_Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Judas_Engine::Renderer2D::DrawQuad({ 1.0f, 0.0f }, glm::vec2(0.5f, 1.0f), { m_SquareColor, 1.0f });
	Judas_Engine::Renderer2D::DrawQuad({ 0.0f, 1.0f }, glm::vec2(1.0f, 0.5f), { 0.5f, 0.1f, 0.8, 1.0f });
	Judas_Engine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Judas_Engine::Event& e)
{
	m_CameraController.OnEvent(e);
}
