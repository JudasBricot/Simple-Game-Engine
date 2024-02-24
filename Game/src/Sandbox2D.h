#pragma once

#include <JudasEngine.h>

class Sandbox2D : public Judas_Engine::Layer
{
public:
	Sandbox2D();
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Judas_Engine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Judas_Engine::Event& e) override;
private:
	Judas_Engine::ShaderLibrary m_ShaderLibrary;

	Judas_Engine::Ref<Judas_Engine::Texture2D> m_Texture;

	Judas_Engine::Ref<Judas_Engine::Shader> m_FlatColorShader;
	Judas_Engine::Ref<Judas_Engine::VertexArray> m_VertexArray;

	Judas_Engine::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.1f, 0.8f };
};

