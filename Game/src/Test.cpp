#include <JudasEngine.h>

#include "Judas-Engine/Platform/OpenGL/OpenGLShader.h"
#include "Judas-Engine/Renderer/Shader.h"
#include "Judas-Engine/SceneObjects/Mesh.h"

#include "ImGui/imgui.h"

#ifdef GLM_ENABLE_EXPERIMENTAL
#else
	#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

// temp
#include "glm/ext.hpp"

#include "Assets/Spectrum.h"
#include "Assets/Ocean.h"

#include <iostream>

class ExampleLayer : public Judas_Engine::Layer
{
public:
	ExampleLayer()
		: Layer("MyLayer"), m_CameraController(1.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		uint32_t size = 256;
		SpectrumInfos infos = {1.0f, glm::vec2(31.0f, 0.0f), 9.8f, 1.0f};
		Spectrum ps(infos, size, size);
		ps.GenerateFourierAmplitude();
		std::vector<std::complex<float>> spectrum = ps.GetSpectrum();
		//Ocean ocean(infos, size, size);
		//ocean.Generate();
		//std::vector<float> heightField = ocean.GetRealHeightField();

		std::vector<unsigned char> spectrum_ui(size * size * 3);

		for (size_t i = 0; i < size * size; i++)
		{
			float clamped_value = spectrum[i].real() > 1.0 ? 1.0f : spectrum[i].real();
			spectrum_ui[3 * i] = char(255 * clamped_value);
			spectrum_ui[3 * i + 1] = char(255 * clamped_value);
			spectrum_ui[3 * i + 2] = char(255 * clamped_value);
		}

		//m_Texture = Judas_Engine::Texture2D::Create("src/Assets/textures/texture.png");
		m_Texture = Judas_Engine::Texture2D::Create(spectrum_ui.data(), size, size, 3);

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Judas_Engine::RenderCommand::Clear();

		Judas_Engine::Renderer::BeginScene(m_CameraController.GetCamera());

		m_Texture->Bind(0);
		m_Mesh->Submit();

		Judas_Engine::Renderer::EndScene();
	}

	void OnEvent(Judas_Engine::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref<Judas_Engine::Texture2D> m_Texture, m_ChernoLogoTexture;

	Judas_Engine::Ref<Judas_Engine::VertexArray> m_VertexArray;
	glm::mat4x4 position;

	Judas_Engine::PerspectiveCameraController m_CameraController;
	Judas_Engine::Ref < Judas_Engine::Mesh> m_Mesh;
};

class CubeLayer : public Judas_Engine::Layer
{
public:
	CubeLayer()
		: Layer("MyLayer"), m_CameraController(1.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/colors.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Judas_Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Judas_Engine::RenderCommand::Clear();

		Judas_Engine::Renderer::BeginScene(m_CameraController.GetCamera());

		m_Mesh->Submit();

		Judas_Engine::Renderer::EndScene();
	}

	void OnEvent(Judas_Engine::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::PerspectiveCameraController m_CameraController;

	std::vector<unsigned int> m_Indices;
	std::vector<float> m_Vertices;
};

class GameApplication : public Judas_Engine::Application
{
public:
	GameApplication()
	{
		PushLayer(new ExampleLayer());

		/*m_CameraLayer = new CameraLayer(&m_Camera);
		PushLayer(m_CameraLayer);*/
	}
	~GameApplication()
	{

	}
	private:
};

Judas_Engine::Application* Judas_Engine::CreateApplication()
{
	return new GameApplication();
}