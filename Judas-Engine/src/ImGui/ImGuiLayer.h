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

		void OnUpdate() override;
		void OnAttach() override;
		void OnDetach() override;

		void OnEvent(Event& e) override;
	private:
		bool OnMouseMovedEvent(MouseMovedEvent e);
		bool OnMouseScrolledEvent(MouseScrolledEvent e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent e);
		bool OnKeyPressedEvent(KeyPressedEvent e);
		bool OnKeyReleasedEvent(KeyReleasedEvent e);
		bool OnKeyTypedEvent(KeyTypedEvent e);
		bool OnWindowResizedEvent(WindowResizedEvent e);
	private:
		float m_Time = 0.0f;
	};
}
