#pragma once

#include <JudasEngine.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

enum class EnveloppeSpectrum { Phillips };
enum class DirectionalSpectrum { None };

template <unsigned int SIZE>
class SpectrumGenerator
{
	struct Ph_Data
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
	};

public:
	SpectrumGenerator(EnveloppeSpectrum enveloppeSpectrum = EnveloppeSpectrum::Phillips, DirectionalSpectrum directionalSpectrum = DirectionalSpectrum::None);
	~SpectrumGenerator();

	void ComputeInitialSpectrum();
	void ComputeTimeDependentSpectrum(float t);

	const Judas_Engine::Ref<Judas_Engine::RenderTexture2D> GetSpectrumTexture() { return m_SpectrumTexture; }
	const Judas_Engine::Ref<Judas_Engine::RenderTexture2D> GetSlopeSpectrumTexture() { return m_SlopeSpectrumTexture; }
	const Judas_Engine::Ref<Judas_Engine::RenderTexture2D> GetDisplacementSpectrumTexture() { return m_DisplacementSpectrumTexture; }

	void ImGuiRender()
	{
		ImGui::DragFloat2("Step", glm::value_ptr(m_SpectrumData->Step), 0.001f, 0.0f, 5.0f);
		ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_SpectrumData->Wind_direction), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Amplitude", &m_SpectrumData->Amplitude, 0.001f, 0.1f, 2.0f);
		ImGui::DragFloat("Wind Amplitude", &m_SpectrumData->Wind_amplitude, 0.01f, 0.0f, 60.0f);
		ImGui::DragFloat("Gravity", &m_SpectrumData->Gravity, 0.001f, 0.0f, 1.0f);
		ImGui::DragFloat("Min Wave Height Sqrd", &m_SpectrumData->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
		m_SpectrumData->UpdateData();
	}

private:
	EnveloppeSpectrum m_EnveloppeSpectrum;
	DirectionalSpectrum m_DirectionalSpectrum;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeSpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementSpectrumTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_SpectrumComputeShader;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_SpectrumDataBuffer;
	Judas_Engine::Ref<Ph_Data> m_SpectrumData;
};

// Impl

template <unsigned int SIZE>
SpectrumGenerator<SIZE>::SpectrumGenerator(EnveloppeSpectrum enveloppeSpectrum, DirectionalSpectrum directionalSpectrum) : m_EnveloppeSpectrum(enveloppeSpectrum), m_DirectionalSpectrum(directionalSpectrum)
{
	// Texture initialization
	m_SpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);
	m_SlopeSpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);
	m_DisplacementSpectrumTexture = Judas_Engine::RenderTexture2D::Create(SIZE + 1, SIZE + 1);

	// Cmopute Shader initialization
	m_SpectrumComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/phillipsSpectrum.glsl");

	// Initialize Spectrum Data
	m_SpectrumData = std::make_shared<Ph_Data>(
		1.0f,
		glm::vec2(1.0f, 0.0f),
		31.0f,
		9.8f,
		0.1f,
		glm::vec2(1.0f)
	);
	m_SpectrumData->Time = 46832.654f;
	m_SpectrumDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_SpectrumData, sizeof(Ph_Data));
}

template<unsigned int SIZE>
inline SpectrumGenerator<SIZE>::~SpectrumGenerator()
{
}

template<unsigned int SIZE>
inline void SpectrumGenerator<SIZE>::ComputeInitialSpectrum()
{
}

template<unsigned int SIZE>
inline void SpectrumGenerator<SIZE>::ComputeTimeDependentSpectrum(float deltaTime)
{
	m_SpectrumTexture->BindImage(0);
	m_SlopeSpectrumTexture->BindImage(1);
	m_DisplacementSpectrumTexture->BindImage(2);

	m_SpectrumDataBuffer->Bind(3);
	m_SpectrumData->UpdateTime(deltaTime);
	m_SpectrumDataBuffer->UpdateData((Judas_Engine::Ref<void>)m_SpectrumData);

	m_SpectrumComputeShader->Dispatch(SIZE / 16 + 1, SIZE / 16 + 1, 1);

	m_SpectrumTexture->UnbindImage();
	m_SlopeSpectrumTexture->UnbindImage();
	m_DisplacementSpectrumTexture->UnbindImage();

	m_SpectrumDataBuffer->Unbind();
}

