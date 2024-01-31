#pragma once

#include <JudasEngine.h>

#include <Judas-Engine/Platform/OpenGL/OpenGLBuffer.h>

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
class IFFT2D
{
	struct FFTData
	{
		int stage = 0;
		int pingpong = 0;
		int direction = 0;
	};

	struct NormalizationData
	{
		int pingpong = 0;
	};

public:
	IFFT2D(const Judas_Engine::Ref<Judas_Engine::RenderTexture2D>& inputTexture);
	~IFFT2D() {}

	void Compute();

	const Judas_Engine::Ref<Judas_Engine::RenderTexture2D> GetOutputTexture() { return m_NormalizationRenderTexture; ComputeButterfly(); }
private:
	void ComputeButterfly();
	void ComputeFFT();
	void ComputeFFTStage(int stage, int pingpong, int direction);
	void NormalizeFFT();
private:
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_InputTexture;

	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_ButterflyRenderTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_PingpongRenderTexture;
	Judas_Engine::Ref<Judas_Engine::RenderTexture2D> m_NormalizationRenderTexture;

	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyTextureComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_ButterflyPassComputeShader;
	Judas_Engine::Ref <Judas_Engine::ComputeShader> m_NormalizationComputeShader;

	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_ButterflyPassDataBuffer;
	Judas_Engine::Ref<Judas_Engine::OpenGLDataBufferObject> m_NormalizationDataBuffer;

	Judas_Engine::Ref<FFTData> m_ButterflyPassData;
	Judas_Engine::Ref<NormalizationData> m_Inversion_Data;
};

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
IFFT2D<SIZE, LOG_2_SIZE>::IFFT2D(const Judas_Engine::Ref<Judas_Engine::RenderTexture2D>& inputTexture) : m_InputTexture(inputTexture)
{
	// --------------------------------
	// BUTTERFLY TEXTURE INITIALIZATION
	// --------------------------------

	m_ButterflyRenderTexture = Judas_Engine::RenderTexture2D::Create(SIZE, LOG_2_SIZE);
	m_ButterflyTextureComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/butterflyTexture.glsl");

	// Init butterfly texture
	ComputeButterfly();

	// ------------------
	// FFT INITIALIZATION
	// ------------------

	m_PingpongRenderTexture = Judas_Engine::RenderTexture2D::Create(SIZE, SIZE);
	m_ButterflyPassComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/butterfly.glsl");

	m_ButterflyPassData = std::make_shared<FFTData>();
	m_ButterflyPassDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>((Judas_Engine::Ref<void>)m_ButterflyPassData, sizeof(FFTData));

	// ----------------------------
	// NORMALIZATION INITIALIZATION
	// ----------------------------

	m_NormalizationRenderTexture = Judas_Engine::RenderTexture2D::Create(SIZE, SIZE);
	m_NormalizationComputeShader = Judas_Engine::ComputeShader::Create("src/Assets/ComputeShaders/permutation.glsl");

	m_Inversion_Data = std::make_shared<NormalizationData>();
	m_NormalizationDataBuffer = std::make_shared<Judas_Engine::OpenGLDataBufferObject>(m_Inversion_Data, sizeof(NormalizationData));
}

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
void IFFT2D<SIZE, LOG_2_SIZE>::Compute()
{
	ComputeFFT();
	NormalizeFFT();
}


template<unsigned int SIZE, unsigned int LOG_2_SIZE>
void IFFT2D<SIZE, LOG_2_SIZE>::ComputeButterfly()
{
	m_ButterflyRenderTexture->Bind(0);
	m_ButterflyTextureComputeShader->Dispatch(SIZE / 2, LOG_2_SIZE, 1);
	m_ButterflyRenderTexture->Unbind();
}

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
void IFFT2D<SIZE, LOG_2_SIZE>::ComputeFFT()
{
	m_ButterflyRenderTexture->Bind(0);
	m_InputTexture->Bind(1);
	m_PingpongRenderTexture->Bind(2);
	m_ButterflyPassDataBuffer->Bind(3);

	for (int direction = 0; direction < 2; direction++)
	{
		m_ButterflyPassData->direction = direction;
		for (int stage = 0; stage < LOG_2_SIZE; stage++)
			ComputeFFTStage(stage, stage % 2, direction);
	}

	m_ButterflyRenderTexture->Unbind();
	m_InputTexture->Unbind();
	m_PingpongRenderTexture->Unbind();

	m_ButterflyPassDataBuffer->Unbind();
}

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
void IFFT2D<SIZE, LOG_2_SIZE>::ComputeFFTStage(int stage, int pingpong, int direction)
{
	// Update Info
	m_ButterflyPassData->direction = direction;
	m_ButterflyPassData->stage = stage;
	m_ButterflyPassData->pingpong = pingpong;
	m_ButterflyPassDataBuffer->UpdateData(m_ButterflyPassData);

	m_ButterflyPassComputeShader->Dispatch(SIZE / 16, SIZE / 16, 1);
}

template<unsigned int SIZE, unsigned int LOG_2_SIZE>
void IFFT2D<SIZE, LOG_2_SIZE>::NormalizeFFT()
{
	m_NormalizationRenderTexture->Bind(0);
	m_InputTexture->Bind(1);
	m_PingpongRenderTexture->Bind(2);
	m_NormalizationDataBuffer->Bind(3);

	m_NormalizationComputeShader->Dispatch(SIZE / 16, SIZE / 16, 1);

	m_NormalizationRenderTexture->Unbind();
	m_InputTexture->Unbind();
	m_PingpongRenderTexture->Unbind();
	m_NormalizationDataBuffer->Unbind();
}
