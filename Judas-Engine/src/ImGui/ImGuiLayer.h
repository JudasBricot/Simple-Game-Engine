#pragma once

#include "Core.h"

#include "Layers/Layer.h"
#include "EventSystem/Event.h"

namespace Judas_Engine
{
	class JE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUpdate() override;
		void OnEvent(Event& e) override;

		void OnAttach() override;
		void OnDetach() override;
	private:
		float m_Time = 0.0f;
	};
}
