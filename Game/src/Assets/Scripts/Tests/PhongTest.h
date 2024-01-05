#pragma once

#include <JudasEngine.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>

class PhongLayer : public Judas_Engine::Layer
{
	struct Light
	{
		glm::vec3 Direction;

		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;

		void UpdateUniforms(Judas_Engine::Ref<Judas_Engine::Shader> shader)
		{
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.Direction", Direction);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.AmbientColor", AmbientColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.DiffuseColor", DiffuseColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Light.SpecularColor", SpecularColor);
		}
	};

	struct Material
	{
		glm::vec3 DiffuseColor;
		float DiffuseReflection;

		glm::vec3 SpecularColor;
		float SpecularReflection;
		float SpecularShininess;

		void UpdateUniforms(Judas_Engine::Ref<Judas_Engine::Shader> shader)
		{
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Material.DiffuseColor", DiffuseColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.DiffuseReflection", DiffuseReflection);

			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat3("u_Material.SpecularColor", SpecularColor);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.SpecularReflection", SpecularReflection);
			std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(shader)->UploadUniformFloat("u_Material.SpecularShininess", SpecularShininess);
		}
	};

public:
	PhongLayer()
		: Layer("MyLayer") {}

	virtual void OnAttach()
	{
		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/phong.glsl");
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(m_PrimitiveType, 64);
		m_Mesh->SetShader(m_Shader);
	}

	void SetPrimitiveType(const Judas_Engine::PrimitiveType& primitiveType)
	{
		if (primitiveType == m_PrimitiveType)
			return;

		m_PrimitiveType = primitiveType;
		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(m_PrimitiveType, 64);
		m_Mesh->SetShader(m_Shader);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Primitive Type");
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


		ImGui::Begin("Light properties");

		ImGui::DragFloat3("Direction", glm::value_ptr(m_Light.Direction), 0.001f, -1.0f, 1.0f);
		ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_Light.AmbientColor));
		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_Light.DiffuseColor));
		ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_Light.SpecularColor));

		ImGui::End();


		ImGui::Begin("Material properties");

		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_Material.DiffuseColor));
		ImGui::DragFloat("Diffuse reflection", &m_Material.DiffuseReflection, 0.001f, 0.0f, 10.0f);
		ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_Material.SpecularColor));
		ImGui::DragFloat("Specular reflection", &m_Material.SpecularReflection, 0.001f, 0.0f, 10.0f);
		ImGui::DragFloat("Specular shininess", &m_Material.SpecularShininess, 0.001f, 0.0f, 2.0f);

		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		m_Light.UpdateUniforms(m_Shader);
		m_Material.UpdateUniforms(m_Shader);

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;

	Judas_Engine::PrimitiveType m_PrimitiveType = Judas_Engine::PrimitiveType::Sphere;

	Light m_Light = { glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f) };
	Material m_Material = { glm::vec3(0.0f, 0.0f, 0.2f), 0.7f, glm::vec3(1.0f), 0.9f, 16.0f };
};
