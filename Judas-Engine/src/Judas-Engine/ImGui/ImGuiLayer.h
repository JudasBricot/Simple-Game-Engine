#pragma once

#include "Judas-Engine/Core/Core.h"

#include "Judas-Engine/Layers/Layer.h"

#include "Judas-Engine/EventSystem/Event.h"
#include "Judas-Engine/EventSystem/ApplicationEvent.h"

namespace Judas_Engine
{
	class JE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
