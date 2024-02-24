#pragma once
#include <string>

#include <glm/glm.hpp>

namespace Judas_Engine
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual const std::string GetName() = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetMat4(const char* name, glm::mat4 mat) = 0;
		virtual void SetFloat4(const char* name, glm::vec4 vec) = 0;
		virtual void SetFloat3(const char* name, glm::vec3 vec) = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}

