#include "Spectrum.h"

#include <glm/glm.hpp>

#include <math.h>
#include <complex>
#include <Judas-Engine/Logging/Log.h>

static float gaussian(std::uniform_real_distribution<>& rand)
{
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());

    double u1 = 1.0 - rand(gen); //uniform(0,1] random doubles
    double u2 = 1.0 - rand(gen);
    return sqrt(-2.0f * log((float)u1)) * sin(2.0f * PI * (float)u2); //random normal(0,1)
}

Spectrum::Spectrum(const SpectrumInfos& spectrumInfos, int width, int height) : m_Width(width), m_Height(height)
{
    m_Amplitude = spectrumInfos.amplitude;
    m_Gravity = spectrumInfos.gravity;
    m_MinimumWaveHeightSqrd = spectrumInfos.minWaveHeight;
    m_Wind = spectrumInfos.wind;

    m_NormalizedWind = glm::normalize(m_Wind);
    m_WindSqrd = m_Wind.x * m_Wind.x + m_Wind.y * m_Wind.y ;

    m_L = m_WindSqrd / m_Gravity;
    m_LSqrd = m_L * m_L;

    m_Spectrum.resize(m_Width * m_Height);
}

void Spectrum::GeneratePhillips()
{
    for (int i = 0; i < m_Width; i++)
    {
        for (int j = 0; j < m_Height; j++)
        {
            glm::vec2 k = glm::vec2(i - (int)m_Width / 2, j - (int)m_Height / 2) * 2.0f * (float)PI / m_L;
            float k_sqrd = (k.x * k.x + k.y * k.y);

            float dotProd = dot(glm::normalize(k), m_NormalizedWind);
            float amp = m_Amplitude * dotProd * dotProd;
            float ex = exp(-1.0f / (k_sqrd * m_LSqrd));
            float scale = exp(-k_sqrd * m_MinimumWaveHeightSqrd);

            m_Spectrum[i * m_Height + j] = std::complex<float>(amp * ex * scale / (k_sqrd * k_sqrd));
        }
    }
}

void Spectrum::GenerateFourier()
{
    std::uniform_real_distribution<> rand(0.0, 1.0);

    for (int i = 0; i < m_Width; i++)
    {
        for (int j = 0; j < m_Height; j++)
        {
            glm::vec2 k = glm::vec2(i - (int)m_Width / 2, j - (int)m_Height / 2) * 2.0f * (float)PI / m_L;
            float phSpectrumValue = PhillipsSpectrum(k);

            glm::vec2 epsilon(gaussian(rand), gaussian(rand));

            glm::vec2 h0Tilde = 1.0f / sqrt(2.0f) * epsilon * sqrt(phSpectrumValue);
            m_Spectrum[i * m_Height + j] = std::complex<float>((float)h0Tilde.x, (float)h0Tilde.y);
        }
    }
}

void Spectrum::GenerateFourierAmplitude()
{
    std::uniform_real_distribution<> rand(0.0, 1.0);

    for (int i = 0; i < m_Width; i++)
    {
        for (int j = 0; j < m_Height; j++)
        {
            glm::vec2 k = glm::vec2(i - (int)m_Width / 2, j - (int)m_Height / 2) * 2.0f * (float)PI / m_L;
            float phSpectrumValue = PhillipsSpectrum(k);
            float omega = sqrt(m_Gravity * sqrt(k.x * k.x + k.y * k.y));

            glm::vec2 epsilon(gaussian(rand), gaussian(rand));
            glm::vec2 h0Tilde = 1.0f / sqrt(2.0f) * epsilon * sqrt(phSpectrumValue);

            glm::vec2 fourierAmplitude = glm::vec2((h0Tilde.x + h0Tilde.y) * cos(omega), (h0Tilde.y - h0Tilde.x) * sin(omega));

            
            m_Spectrum[i * m_Height + j] = std::complex<float>((float)fourierAmplitude.x, (float)fourierAmplitude.y);
        }
    }
}

float Spectrum::PhillipsSpectrum(const glm::vec2& k)
{
    float k_sqrd = (k.x * k.x + k.y * k.y);

    float dotProd = dot(glm::normalize(k), m_NormalizedWind);
    float amp = m_Amplitude * dotProd * dotProd;
    float ex = exp(-1.0f / (k_sqrd * m_LSqrd));
    float scale = exp(-k_sqrd * m_MinimumWaveHeightSqrd);

    return amp * ex * scale / (k_sqrd * k_sqrd);
}
