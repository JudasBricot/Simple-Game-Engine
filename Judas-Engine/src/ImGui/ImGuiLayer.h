#pragma once

#include "Core.h"

#include "Layers/Layer.h"

#include "EventSystem/Event.h"
#include "EventSystem/ApplicationEvent.h"

namespace Judas_Engine
{
	class JE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
