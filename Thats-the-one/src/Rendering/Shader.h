#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	void SetUniformMat4fvLocation(const std::string& name, const glm::mat4& mat);

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, unsigned int> m_UniformLocations;

	ShaderSource ParseShader(const std::string filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

