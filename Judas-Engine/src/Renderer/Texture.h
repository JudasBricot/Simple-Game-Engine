#pragma once

#include <string>

#include "Core.h"


namespace Judas_Engine
{
	class Texture
	{
	public:
		~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t Getheight() const = 0;

		virtual void Bind(uint32_t slot) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}
