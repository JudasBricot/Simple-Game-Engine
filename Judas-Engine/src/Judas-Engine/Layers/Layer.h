#pragma once

#include "Judas-Engine/EventSystem/Event.h"

#include "Judas-Engine/Core/Core.h"
#include "Judas-Engine/Core/Timestep.h"

namespace Judas_Engine
{
	class JE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}


