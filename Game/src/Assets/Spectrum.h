#pragma once

#include <vector>
#include <random>
#include <complex>
#include <glm/glm.hpp>

#include <iostream>

#define PI 3.14159265358979323846

struct SpectrumInfos
{
    float amplitude;
    glm::vec2 wind;
    float gravity;
    float minWaveHeight;
};

class Spectrum
{
public:
    Spectrum(const SpectrumInfos& spectrumInfos, int width, int height);
    void GeneratePhillips();
    void GenerateFourier();
    void GenerateFourierAmplitude();
    const std::vector<std::complex<float>>& GetSpectrum() const { return m_Spectrum; }
private:
    float PhillipsSpectrum(const glm::vec2& k);
protected:
    uint32_t m_Width, m_Height;

    std::vector<std::complex<float>> m_Spectrum;

    float m_Amplitude, m_Gravity, m_MinimumWaveHeightSqrd, m_L;
    glm::vec2 m_Wind, m_NormalizedWind;
    float m_LSqrd, m_WindSqrd;
};

/*class PhillipsSpectrum : public Spectrum
{
public:
    PhillipsSpectrum(const SpectrumInfos& spectrumInfos, int width, int height) : Spectrum(spectrumInfos, width, height) {}
    virtual void Generate() override;
};*/

/*
static float gaussian(std::uniform_real_distribution<>& rand)
{
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());

    double u1 = 1.0 - rand(gen); //uniform(0,1] random doubles
    double u2 = 1.0 - rand(gen);
    return sqrt(-2.0f * log((float)u1)) * sin(2.0f * PI * (float)u2); //random normal(0,1)
}

static float GeneratePhillipsSpectrum(SpectrumInfos spectrumInfos, glm::vec2 k)
{
    float L = spectrumInfos.wind.length() / spectrumInfos.gravity;
    float k_sqrd = k.length();

    return spectrumInfos.amplitude * abs(dot(glm::normalize(k), glm::normalize(spectrumInfos.wind))) * exp(-1.0f / (k_sqrd * L * L) - k_sqrd * spectrumInfos.minWaveHeight * spectrumInfos.minWaveHeight) / (k_sqrd * k_sqrd);

}

static std::vector<float> GeneratePhillipsSpectrum(SpectrumInfos spectrumInfos, int width, int height)
{
    std::vector<float> spectrum(width * height);
    float L = spectrumInfos.wind.length() / spectrumInfos.gravity;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            glm::vec2 k = 7.0f * glm::vec2(i - width / 2, j - height / 2) / 256.0f;
            float k_sqrd = k.length();

            spectrum.push_back(spectrumInfos.amplitude * abs(dot(glm::normalize(k), glm::normalize(spectrumInfos.wind))) * exp(-1.0f / (k_sqrd * L * L) - k_sqrd * spectrumInfos.minWaveHeight * spectrumInfos.minWaveHeight) / (k_sqrd * k_sqrd));
        }
    }

    return spectrum;
}

static std::vector<float> GenerateFourierAmplitude(SpectrumInfos spectrumInfos, int width, int height)
{
    std::vector<float> spectrum(width * height);
    std::uniform_real_distribution<> rand(0.0, 1.0);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            glm::vec2 k = 7.0f * glm::vec2(i - width / 2, j - height / 2) / 256.0f;

            float php_spectrum = GeneratePhillipsSpectrum(spectrumInfos, k);
            glm::vec2 epsilon(gaussian(rand), gaussian(rand));

            glm::vec2 h0Tilde = 1.0f / sqrt(2.0f) * epsilon * sqrt(php_spectrum);
            spectrum.push_back((float)h0Tilde.x);
        }
    }

    return spectrum;
}

static glm::vec2 GenerateFourierAmplitude(SpectrumInfos spectrumInfos, glm::vec2 k)
{
    float php_spectrum = GeneratePhillipsSpectrum(spectrumInfos, k);
    std::uniform_real_distribution<> rand(0.0f, 1.0f);
    glm::vec2 epsilon(gaussian(rand), gaussian(rand));

    glm::vec2 h0Tilde = 1.0f / sqrt(2.0f) * epsilon * sqrt(php_spectrum);

    return h0Tilde;
}

static std::vector<float> GenerateWaveAmplitudeSpectrum(SpectrumInfos spectrumInfos, int width, int height)
{
    std::vector<float> spectrum;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            glm::vec2 k = 7.0f * glm::vec2(i - width / 2, j - height / 2) / (float)width;
            glm::vec2 h0Tilde_k = GenerateFourierAmplitude(spectrumInfos, k);
            glm::vec2 h0Tilde_mk = GenerateFourierAmplitude(spectrumInfos, -k);

            float omega = spectrumInfos.gravity * sqrt(k.length());
            glm::vec2 exp_k(cos(omega), sin(omega));
            glm::vec2 exp_mk(cos(omega), -sin(omega));

            glm::vec2 res = h0Tilde_k * exp_k + h0Tilde_mk * exp_mk;
            spectrum.push_back(res.x);
            std::cout << res.x << std::endl;
        }
    }

    return spectrum;
}


/*using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Numerics;
using Vector2 = UnityEngine.Vector2;

public class WaveSpectrumGeneration : MonoBehaviour
{
    [System.Serializable]
    public struct SpectrumInfos
    {
        public float amplitude;
        public Vector2 wind;
        public float gravity;
        public float minWaveHeight;
    }

    public SpectrumInfos spectrumInfos;

    private Texture2D tex;

    const float PI = Mathf.PI;

    [System.Serializable] enum ValueType { PhillipsSpectrum, FourierAmplitude, WaveHeightSpectrum, waveHeight};
    [SerializeField] ValueType spectrumType;

    private static float abs(float a) => a > 0 ? a : -a;

    private static float dot(Vector2 a, Vector2 b) => a.x * b.x + a.y * b.y;

    private static float sqrt(float a) => Mathf.Sqrt(a);

    private static float exp(float a) => Mathf.Exp(a);

    private static float log(float a) => Mathf.Log(a);

    private static float sin(float a) => Mathf.Sin(a);

    private static float cos(float a) => Mathf.Cos(a);

    private static float gaussian(System.Random rand)
    {
        double u1 = 1.0 - rand.NextDouble(); //uniform(0,1] random doubles
        double u2 = 1.0 - rand.NextDouble();
        return sqrt(-2.0f * log((float)u1)) * sin(2.0f * PI * (float)u2); //random normal(0,1)
    }

    private void Awake()
    {
        float[,] spectrum = WaveSpectrum(256, 256, spectrumInfos, spectrumType);
        tex = GenerateTexture(spectrum);
        GetComponent<MeshRenderer>().sharedMaterial.SetTexture("_MainTex", tex);
    }

    private static float PhillipsSpectrum(SpectrumInfos spectrumInfos, Vector2 k)
    {
        float k_sqrd = k.sqrMagnitude;
        float L = spectrumInfos.wind.sqrMagnitude / spectrumInfos.gravity;

        return spectrumInfos.amplitude * abs(dot(k.normalized, spectrumInfos.wind.normalized)) * Mathf.Exp(-1.0f / (k_sqrd * L * L) - k_sqrd * spectrumInfos.minWaveHeight * spectrumInfos.minWaveHeight) / (k_sqrd * k_sqrd);
    }

    private static float[,] PhillipsSpectrum(SpectrumInfos spectrumInfos, int width, int height)
    {
        float[,] spectrum = new float[width, height];
        float L = spectrumInfos.wind.sqrMagnitude / spectrumInfos.gravity;

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Vector2 k = 7.0f * new Vector2(i - width / 2, j - height / 2) / 256.0f;
                float k_sqrd = k.sqrMagnitude;

                spectrum[i, j] = spectrumInfos.amplitude * abs(dot(k.normalized, spectrumInfos.wind.normalized)) * Mathf.Exp(-1.0f / (k_sqrd * L * L) - k_sqrd * spectrumInfos.minWaveHeight * spectrumInfos.minWaveHeight) / (k_sqrd * k_sqrd);
            }
        }

        return spectrum;
    }

    private static Complex FourierAmplitude(SpectrumInfos spectrumInfos, Vector2 k)
    {
        float php_spectrum = PhillipsSpectrum(spectrumInfos, k);
        System.Random rand = new System.Random();
        Complex epsilon = new Complex(gaussian(rand), gaussian(rand));

        Complex h0Tilde = 1.0f / sqrt(2) * epsilon * sqrt(php_spectrum);

        return h0Tilde;
    }

    private static float[,] FourierAmplitude(SpectrumInfos spectrumInfos, int width, int height)
    {
        float[,] spectrum = new float[width, height];
        System.Random rand = new System.Random();

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Vector2 k = 7.0f * new Vector2(i - width / 2, j - height / 2) / 256.0f;

                float php_spectrum = PhillipsSpectrum(spectrumInfos, k);
                Complex epsilon = new Complex(gaussian(rand), gaussian(rand));

                Complex h0Tilde = 1.0f / sqrt(2) * epsilon * sqrt(php_spectrum);
                spectrum[i, j] = (float)h0Tilde.Real;
            }
        }

        return spectrum;
    }

    private static Complex WaveAmplitudeSpectrum(SpectrumInfos spectrumInfos, Vector2 k)
    {
        Complex h0Tilde_k = FourierAmplitude(spectrumInfos, k);
        Complex h0Tilde_mk = FourierAmplitude(spectrumInfos, -k);

        float omega = spectrumInfos.gravity * k.magnitude;
        Complex exp_k = new Complex(cos(omega), sin(omega));
        Complex exp_mk = new Complex(cos(omega), - sin(omega));

        return h0Tilde_k * exp_k + h0Tilde_mk * exp_mk;
    }

    private static Complex[,] WaveAmplitudeSpectrum(SpectrumInfos spectrumInfos, int width, int height)
    {
        Complex[,] spectrum = new Complex[width, height];

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Vector2 k = 7.0f * new Vector2(i - width / 2, j - height / 2) / 256.0f;
                Complex h0Tilde_k = FourierAmplitude(spectrumInfos, k);
                Complex h0Tilde_mk = FourierAmplitude(spectrumInfos, -k);

                float omega = spectrumInfos.gravity * k.magnitude;
                Complex exp_k = new Complex(cos(omega), sin(omega));
                Complex exp_mk = new Complex(cos(omega), -sin(omega));

                Complex res = h0Tilde_k * exp_k + h0Tilde_mk * exp_mk;
                spectrum[i, j] = res;
            }
        }

        return spectrum;
    }

    private static float[,] WaveAmplitudeSpectrumReal(SpectrumInfos spectrumInfos, int width, int height)
    {
        float[,] spectrum = new float[width, height];

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Vector2 k = 7.0f * new Vector2(i - width / 2, j - height / 2) / 256.0f;
                Complex h0Tilde_k = FourierAmplitude(spectrumInfos, k);
                Complex h0Tilde_mk = FourierAmplitude(spectrumInfos, -k);

                float omega = spectrumInfos.gravity * k.magnitude;
                Complex exp_k = new Complex(cos(omega), sin(omega));
                Complex exp_mk = new Complex(cos(omega), -sin(omega));

                Complex res = h0Tilde_k * exp_k + h0Tilde_mk * exp_mk;
                spectrum[i, j] = (float)res.Real;
            }
        }

        return spectrum;
    }


    private static void WaveHeight(SpectrumInfos spectrumInfos, int width, int height)
    {
        //Complex[,] complexSpectrum = WaveAmplitudeSpectrum(spectrumInfos, width, height);

        //Complex[,] complexHeight = Fourier.IFFT2D(complexSpectrum);

        //return Fourier.GetRealArray(complexHeight);
        int N = 2;
        Complex[] vec = new Complex[N];
        for (int i = 0; i < N; i++)
        {
            vec[i] = new Complex(Math.Cos(2.0 * Math.PI * (double)i / (double)N), Math.Sin(2.0 * Math.PI * (float)i / (double)N));
        }

        //vec = Fourier.FFT(vec);
        for (int i = 0; i < N; i++)
        {
            Debug.Log(vec[i]);
        }

        /*Complex[,] vec = new Complex[width,height];
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                vec[i,j] = new Complex(Mathf.Cos(2.0f * Mathf.PI * (float)((i - width / 2.0f) + (j - height / 2.0f)) / 10.0f), 0.0f);
            }
        }

        return Fourier.GetRealArray(Fourier.FFT2D(vec));*/
        /*}

            private static float[,] WaveSpectrum(int width, int height, SpectrumInfos spectrumInfos, ValueType spectrumType)
            {
                float[,] spectrum = new float[width, height];

                switch(spectrumType)
                {
                    case ValueType.PhillipsSpectrum:
                        spectrum = PhillipsSpectrum(spectrumInfos, width, height);
                        break;
                    case ValueType.FourierAmplitude:
                        spectrum = FourierAmplitude(spectrumInfos, width, height);
                        break;
                    case ValueType.WaveHeightSpectrum:
                        spectrum = WaveAmplitudeSpectrumReal(spectrumInfos, width, height);
                        break;
                    case ValueType.waveHeight:
                        WaveHeight(spectrumInfos, width, height);
                        spectrum = WaveAmplitudeSpectrumReal(spectrumInfos, width, height);

                        break;
                    default:
                        throw new System.NotImplementedException();
                }

                return spectrum;
            }

            private static Texture2D GenerateTexture(float[,] arr)
            {
                int width = arr.GetLength(0), height = arr.GetLength(1);
                Texture2D tex = new Texture2D(width, height);
                Color[] pixels = new Color[width * height];

                for (int i = 0; i < width; i++)
                {
                    for (int j = 0; j < height; j++)
                    {
                        pixels[i + j * width] = new Color(arr[i, j], arr[i, j], arr[i, j]);
                    }
                }

                tex.filterMode = FilterMode.Point;
                tex.SetPixels(pixels);
                tex.Apply();

                return tex;
            }
        }
}
*/