#pragma once

#include <JudasEngine.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>

enum class EnveloppeSpectrum { Phillips, Jonswap, PiersonMoskowitz };
enum class DirectionalSpectrum { None, PhillipsDirectional, SqrdCosine, DonelanBanner };

template <unsigned int SIZE>
class SpectrumGenerator
{
	struct OceanParameters
	{
		glm::vec2 Step;
		glm::vec2 Wind_direction;
		float Wind_amplitude;
		float Gravity;
		float Time;
		int EnveloppeSpectrumIndex;
		int DirectionSpreadingIndex;

		OceanParameters(float amplitude, glm::vec2 wind_direction, float wind_amplitude, float gravity, float min_wave_height_sqrd, glm::vec2 step, int enveloppeSpectrumIndex, int directionSpreadingIndex)
			: Wind_direction(wind_direction), Wind_amplitude(wind_amplitude), Gravity(gravity), Step(step), EnveloppeSpectrumIndex(enveloppeSpectrumIndex), DirectionSpreadingIndex(directionSpreadingIndex)
		{
			Time = 0.0f;
		}

		void UpdateTime(float deltaTime)
		{
			Time += deltaTime;
		}
	};

	struct PhillipsParameters
	{
		float Amplitude;
		float Max_wave_height_sqrd;
		float Min_wave_height_sqrd;
		float AlignementFactor;

		PhillipsParameters(float amplitude, float min_wave_height_sqrd, float alignementFactor, float gravity, float wind_speed)
			: Amplitude(amplitude), Min_wave_height_sqrd(min_wave_height_sqrd), AlignementFactor(alignementFactor)
		{
			UpdateData(wind_speed, gravity);
		}

		void UpdateData(float wind_amplitude, float gravity)
		{
			Max_wave_height_sqrd = wind_amplitude * wind_amplitude / gravity;
			Max_wave_height_sqrd *= Max_wave_height_sqrd;
		}
	};

	struct JonswapParameters
	{
		float Amplitude;
		float Fetch;
		float Gamma;

		JonswapParameters(float amplitude, float fetch, float gamma)
		: Amplitude(amplitude), Fetch(fetch), Gamma(gamma) {}
	};

	struct PiersonParameters
	{
		float Amplitude;

		PiersonParameters(float amplitude)
		: Amplitude(amplitude) {}
	};

	struct TMAParameters
	{
		bool UseTMA;
		float Depth;

		TMAParameters(bool useTMA, float depth)
		: UseTMA(useTMA), Depth(depth) {}
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
		if (ImGui::CollapsingHeader("Spectrum Parameters"))
		{
			if (ImGui::CollapsingHeader("Simulation Parameters"))
			{
				ImGui::DragFloat2("Step", glm::value_ptr(m_SimulationParams->Step), 0.001f, 0.0f, 5.0f);
				ImGui::DragFloat2("Wind Direction", glm::value_ptr(m_SimulationParams->Wind_direction), 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Wind Amplitude", &m_SimulationParams->Wind_amplitude, 0.01f, 0.0f, 60.0f);
				ImGui::DragFloat("Gravity", &m_SimulationParams->Gravity, 0.001f, 0.0f, 1.0f);
			}

			if (ImGui::CollapsingHeader("Non-Directional Spectrum"))
			{
				if (ImGui::Button("Phillips Spectrum"))
				{
					m_EnveloppeSpectrum = EnveloppeSpectrum::Phillips;
					m_SimulationParams->EnveloppeSpectrumIndex = (int)m_EnveloppeSpectrum;
				}

				if (ImGui::Button("Jonswap Spectrum"))
				{
					m_EnveloppeSpectrum = EnveloppeSpectrum::Jonswap;
					m_SimulationParams->EnveloppeSpectrumIndex = (int)m_EnveloppeSpectrum;
				}

				if (ImGui::Button("Pierson-Moskowitz Spectrum"))
				{
					m_EnveloppeSpectrum = EnveloppeSpectrum::PiersonMoskowitz;
					m_SimulationParams->EnveloppeSpectrumIndex = (int)m_EnveloppeSpectrum;
				}

				if (m_EnveloppeSpectrum == EnveloppeSpectrum::Phillips)
				{
					ImGui::DragFloat("Amplitude", &m_PhillipsParams->Amplitude, 0.001f, 0.1f, 2.0f);
					ImGui::DragFloat("Min Wave Height Sqrd", &m_PhillipsParams->Min_wave_height_sqrd, 0.001f, 0.0f, 1.0f);
					ImGui::DragFloat("Max Wave Height Sqrd", &m_PhillipsParams->Max_wave_height_sqrd, 0.001f, 0.1f, 10.0f);
					ImGui::DragFloat("Wave Alignement Factor", &m_PhillipsParams->AlignementFactor, 0.001f, 0.1f, 10.0f);
				}

				if (m_EnveloppeSpectrum == EnveloppeSpectrum::Jonswap)
				{
					ImGui::DragFloat("Amplitude", &m_JonswapParams->Amplitude, 0.001f, 1.0f, 15.0f);
					ImGui::DragFloat("Fetch", &m_JonswapParams->Fetch, 1.0f, 100000.0f, 1000000000.0f);
					ImGui::DragFloat("Gamma", &m_JonswapParams->Gamma, 0.001f, 1.0f, 5.0f);
				}

				if (m_EnveloppeSpectrum == EnveloppeSpectrum::PiersonMoskowitz)
				{
					ImGui::DragFloat("Amplitude", &m_PiersonParams->Amplitude, 0.001f, 1.0f, 15.0f);
				}
			}

			if (ImGui::CollapsingHeader("Directional Spreading Spectrum"))
			{
				if (ImGui::Button("None"))
				{
					m_DirectionalSpectrum = DirectionalSpectrum::None;
					m_SimulationParams->DirectionSpreadingIndex = (int)m_DirectionalSpectrum;
				}

				if (ImGui::Button("Phillips Directional"))
				{
					m_DirectionalSpectrum = DirectionalSpectrum::PhillipsDirectional;
					m_SimulationParams->DirectionSpreadingIndex = (int)m_DirectionalSpectrum;
				}

				if (ImGui::Button("Squared Positive Cosine"))
				{
					m_DirectionalSpectrum = DirectionalSpectrum::SqrdCosine;
					m_SimulationParams->DirectionSpreadingIndex = (int)m_DirectionalSpectrum;
				}

				if (ImGui::Button("Donelan-banner"))
				{
					m_DirectionalSpectrum = DirectionalSpectrum::DonelanBanner;
					m_SimulationParams->DirectionSpreadingIndex = (int)m_DirectionalSpectrum;
				}
			}

			if (ImGui::CollapsingHeader("TMA"))
			{
				ImGui::Checkbox("Use TMA", &m_TMAParams->UseTMA);
				ImGui::DragFloat("Depth", &m_TMAParams->Depth, 0.01f, 1.0f, 10000.0f);
			}
		}
	}

private:
	EnveloppeSpectrum m_EnveloppeSpectrum;
	DirectionalSpectrum m_DirectionalSpectrum;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_SlopeSpectrumTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_DisplacementSpectrumTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_SpectrumComputeShader;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_SimulationParamsBuffer;
	Judas_Engine::Ref<OceanParameters> m_SimulationParams;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_PhillipsParamsBuffer;
	Judas_Engine::Ref<PhillipsParameters> m_PhillipsParams;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_JonswapParamsBuffer;
	Judas_Engine::Ref<JonswapParameters> m_JonswapParams;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_PiersonParamsBuffer;
	Judas_Engine::Ref<PiersonParameters> m_PiersonParams;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_TMAParamsBuffer;
	Judas_Engine::Ref<TMAParameters> m_TMAParams;
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
	m_SimulationParams = std::make_shared<OceanParameters>(
		1.0f,
		glm::vec2(1.0f, 0.0f),
		31.0f,
		9.8f,
		0.1f,
		glm::vec2(1.0f),
		(int)EnveloppeSpectrum::Phillips,
		(int)DirectionalSpectrum::None
	);
	m_SimulationParams->Time = 46832.654f;
	m_SimulationParamsBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_SimulationParams, sizeof(OceanParameters));

	m_PhillipsParams = std::make_shared<PhillipsParameters>(
		1.0f,
		0.1f,
		4.0f,
		m_SimulationParams->Gravity,
		m_SimulationParams->Wind_amplitude
	);
	m_PhillipsParamsBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_PhillipsParams, sizeof(PhillipsParameters));

	m_JonswapParams = std::make_shared<JonswapParameters>(
		1.0f,
		800000.0f,
		3.3
	);
	m_JonswapParamsBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_JonswapParams, sizeof(JonswapParameters));

	m_PiersonParams = std::make_shared<PiersonParameters>(
		1.0f
	);
	m_PiersonParamsBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_PiersonParams, sizeof(PiersonParameters));

	m_TMAParams = std::make_shared<TMAParameters>(
		false,
		0.1f
	);
	m_TMAParamsBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_TMAParams, sizeof(TMAParameters));
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

	m_SimulationParamsBuffer->Bind(3);
	m_SimulationParams->UpdateTime(deltaTime);
	m_SimulationParamsBuffer->UpdateData((Judas_Engine::Ref<void>)m_SimulationParams);

	m_PhillipsParamsBuffer->Bind(4);
	m_SimulationParamsBuffer->UpdateData((Judas_Engine::Ref<void>)m_PhillipsParams);

	m_JonswapParamsBuffer->Bind(5);
	m_JonswapParamsBuffer->UpdateData((Judas_Engine::Ref<void>)m_JonswapParams);

	m_PiersonParamsBuffer->Bind(6);
	m_PiersonParamsBuffer->UpdateData((Judas_Engine::Ref<void>)m_PiersonParams);

	m_TMAParamsBuffer->Bind(7);
	m_TMAParamsBuffer->UpdateData((Judas_Engine::Ref<void>)m_TMAParams);

	m_SpectrumComputeShader->Dispatch(SIZE / 16 + 1, SIZE / 16 + 1, 1);

	m_SpectrumTexture->UnbindImage();
	m_SlopeSpectrumTexture->UnbindImage();
	m_DisplacementSpectrumTexture->UnbindImage();

	m_SimulationParamsBuffer->Unbind();
}

