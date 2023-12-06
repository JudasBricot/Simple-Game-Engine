#pragma once

#include <Assets/Spectrum.h>

class Ocean
{
public:
	Ocean(SpectrumInfos spectrumInfos, int width, int height) : m_Spectrum(spectrumInfos, width, height), m_Width(width), m_Height(height) { m_Spectrum.GenerateFourierAmplitude(); }
	void Generate();
	const std::vector<std::complex<float>>& GetHeightField() { return m_HeightField; }
private:
	Spectrum m_Spectrum;
	int m_Width, m_Height;

	std::vector<std::complex<float>> m_HeightField;
};
