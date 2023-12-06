#include "Spectrum.h"

#include <glm/glm.hpp>

#include <math.h>
#include <Judas-Engine/Logging/Log.h>

void Spectrum::Generate()
{
    m_Spectrum.resize(m_Width * m_Height);
    glm::vec2 w = m_SpectrumInfos.wind;
    float wind_sqrd = (w.x * w.x + w.y * w.y);
    float L = wind_sqrd / m_SpectrumInfos.gravity;

    for (int i = 0; i < m_Width; i++)
    {
        for (int j = 0; j < m_Height; j++)
        {
            glm::vec2 k = glm::vec2(i - (int)m_Width / 2, j - (int)m_Height / 2) * 2.0f * (float)PI / L;
            float k_sqrd = (k.x * k.x + k.y * k.y);

            float dotProd = dot(glm::normalize(k), glm::normalize(w));
            float amp = m_SpectrumInfos.amplitude * dotProd * dotProd;
            float ex = exp(-1.0f / (k_sqrd * L * L));
            float scale = exp(-k_sqrd * m_SpectrumInfos.minWaveHeight * m_SpectrumInfos.minWaveHeight);

            m_Spectrum[i * m_Height + j] = amp * ex * scale / (k_sqrd * k_sqrd);
        }
    }
}
