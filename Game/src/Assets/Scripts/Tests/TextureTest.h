#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLTexture.h>

#include <imgui/imgui.h>

class TextureLayer : public Judas_Engine::Layer
{
public:
	TextureLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/Shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_Texture = Judas_Engine::Texture2D::Create("src/Assets/textures/texture.jpeg");
		std::dynamic_pointer_cast<Judas_Engine::OpenGLTexture2D>(m_Texture);

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Texture->Bind(0);
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::Texture> m_Texture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};