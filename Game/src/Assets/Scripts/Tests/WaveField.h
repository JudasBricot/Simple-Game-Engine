#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Compute/ComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLComputeShader.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLRenderTexture.h>
#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

#include <imgui/imgui.h>

#define SIZE 256
#define LOG_2_SIZE 8

/*struct Ph_Data
{
	glm::vec2 Step;
	glm::vec2 Wind_direction;
	float Amplitude;
	float Wind_amplitude;

	float Gravity;

	float Max_wave_height_sqrd;
	float Min_wave_height_sqrd;

	float Time;

	Ph_Data(float amplitude, glm::vec2 wind_direction, float wind_amplitude, float gravity, float min_wave_height_sqrd, glm::vec2 step)
		: Amplitude(amplitude), Wind_direction(wind_direction), Wind_amplitude(wind_amplitude), Gravity(gravity), Min_wave_height_sqrd(min_wave_height_sqrd), Step(step)
	{
		Max_wave_height_sqrd = Wind_amplitude * Wind_amplitude / Gravity;
		Max_wave_height_sqrd *= Max_wave_height_sqrd;

		Time = 0.0f;
	}

	void UpdateData()
	{
		Max_wave_height_sqrd = Wind_amplitude * Wind_amplitude / Gravity;
		Max_wave_height_sqrd *= Max_wave_height_sqrd;
	}

	void UpdateTime(float deltaTime)
	{
		Time += deltaTime;
	}
};*/


struct Butterfly_Data
{
	int BitReverseTable256[256 * 256] =
	{
	  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
	  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
	  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
	  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
	  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
	  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
	  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
	  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
	  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
	  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
	  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
	  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
	  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
	  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
	  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
	  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
	};
};

struct FFT_Data
{
	int stage = 0;
	int pingpong = 0;
	int direction = 0;
};

class FFTLayer : public Judas_Engine::Layer
{
public:
	FFTLayer()
		: Layer("My Compute Shader Layer") {}

	virtual void OnAttach()
	{
		// SETUP WIND PARAMETERS
		/*m_Data = std::make_shared<Ph_Data>(
			1.0f,
			glm::vec2(1.0f, 0.0f),
			31.0f,
			9.8f,
			0.1f,
			glm::vec2(0.5f)
		);*/

		m_Shader = Judas_Engine::Shader::Create("src/Assets/shaders/texture.glsl");

		m_Mesh = Judas_Engine::Mesh::CreatePrimitive(Judas_Engine::Plane);
		m_Mesh->SetShader(m_Shader);

		m_RenderTexture = Judas_Engine::RenderTexture2D::Create(SIZE, LOG_2_SIZE);
		m_ButterflyTextureComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/butterflyTexture.glsl", m_RenderTexture);

		m_ButterflyData = std::make_shared<Butterfly_Data>();
		m_ButterflyDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_ButterflyData, sizeof(Butterfly_Data));

		// Computation of the Buttefly texture
		std::dynamic_pointer_cast<Judas_Engine::OpenGLComputeShader>(m_ButterflyTextureComputeShader)->Dispatch();

		m_FFTData = std::make_shared<FFT_Data>();
		m_FFTDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_FFTData, sizeof(FFT_Data));
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		/*ImGui::DragFloat2("Step", glm::value_ptr(m_Data->Step), 0.001f, 0.0f, 5.0f);
		ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_Data->Wind_direction), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Amplitude", &m_Data->Amplitude, 0.001f, 0.1f, 2.0f);
		ImGui::DragFloat("Wind Amplitude", &m_Data->Wind_amplitude, 0.01f, 0.0f, 60.0f);
		ImGui::DragFloat("Gravity", &m_Data->Gravity, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("Min Wave Height Sqrd", &m_Data->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
		m_Data->UpdateData();*/
		ImGui::End();
	}

	void OnUpdate(Judas_Engine::Timestep ts) override
	{
		//m_Data->UpdateTime(ts.GetSeconds());

		std::dynamic_pointer_cast<Judas_Engine::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Judas_Engine::OpenGLRenderTexture2D>(m_RenderTexture)->Bind(0);

		m_Mesh->Submit();
	}
private:
	Judas_Engine::Ref<Judas_Engine::Shader> m_Shader;
	Judas_Engine::Ref <Judas_Engine::Mesh> m_Mesh;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_RenderTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyTextureComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyPassComputeShader;
	//Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_WaterDataBuffer;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_ButterflyDataBuffer;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_FFTDataBuffer;

	Judas_Engine::Ref<Butterfly_Data> m_ButterflyData;
	Judas_Engine::Ref<FFT_Data> m_FFTData;

	//Judas_Engine::Ref<Ph_Data> m_Data;
	float time = 0.0f;
};
