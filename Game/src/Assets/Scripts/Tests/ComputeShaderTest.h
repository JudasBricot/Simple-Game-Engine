#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>

#include <imgui/imgui.h>

class ComputeShaderLayer : public Judas_Engine::Layer
{
public:
	ComputeShaderLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		std::string src = R"(
			#version 430 core

			layout (local_size_x = 1, local_size_y = 1) in;
			layout(rgba32f, binding = 0) uniform image2D colorBuffer;

			void main() {
				ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
				ivec2 screenSize = imageSize(colorBuffer);

				vec2 pos = vec2(float(pixelPos.x) / screenSize.x - 0.5, float(pixelPos.y) / screenSize.y - 0.5);
				float color = exp(- (pos.x * pos.x + pos.y * pos.y) / (0.2 * 0.2)) / sqrt(2.0 * 3.141592 * 0.2);

				imageStore(colorBuffer, pixelPos, vec4(color, color, color, 1.0));
			})";

		m_ComputeShader = Judas_Engine::ComputeShader::Create("Compute Shader", src);
		std::dynamic_pointer_cast<Judas_Engine::OpenGLComputeShader>(m_ComputeShader)->InitTexture();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Coucou");
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{

		std::dynamic_pointer_cast<Judas_Engine::OpenGLComputeShader>(m_ComputeShader)->Dispatch();

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLComputeShader>(m_ComputeShader)->BindTexture();

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);
		
		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::Texture> m_Texture;

	unsigned int m_TextureID;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ComputeShader;
};
