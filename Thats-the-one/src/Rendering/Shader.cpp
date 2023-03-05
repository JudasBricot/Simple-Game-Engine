#include "Shader.h"

#include <GL/glew.h>
#include "Renderer.h"

Shader::Shader(const std::string& filePath)
    : m_FilePath(filePath), m_RendererID(0)
{
    ShaderSource source = ParseShader(filePath);//"res/Shader/Basic.shader");
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniformMat4fvLocation(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

ShaderSource Shader::ParseShader(std::string filePath)
{
    enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    ShaderType type = NONE;

    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(sizeof(char) * length);
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile : " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocations.find(name) != m_UniformLocations.end())
    {
        return m_UniformLocations[name];
    }
    int loc = glGetUniformLocation(m_RendererID, name.c_str());
    if (loc == -1)
    {
        std::cout << "Uniform named: " << name << " not found." << std::endl;
    }

    m_UniformLocations[name] = loc;
    return loc;
}