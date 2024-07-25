#pragma once

#include <string>

#include "Judas-Engine/Renderer/Texture.h"

namespace Judas_Engine
{
	class ComputeShader
	{
	public:
		virtual ~ComputeShader() = default;

		virtual const std::string GetName() = 0;

		virtual void Dispatch(int xSize, int ySize, int zSize) const = 0;

		static Ref<ComputeShader> Create(const std::string& filepath);
		static Ref<ComputeShader> Create(const std::string& name, const std::string& src);
	};
}