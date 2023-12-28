#pragma once

#include <string>

#include "Judas-Engine/Core/Core.h"


namespace Judas_Engine
{
	class Texture
	{
	public:
		~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot) = 0;
		virtual void Unbind() const = 0;
	};

	class RenderTexture2D : public Texture
	{
	public:
		static Ref<RenderTexture2D> Create(unsigned int width, unsigned int height);
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const unsigned char* data, int width, int height, int channels);
	};
}
