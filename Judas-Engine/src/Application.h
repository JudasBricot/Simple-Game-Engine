#pragma once

#include <iostream>

#include "Window.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "EventSystem/ApplicationEvent.h"

namespace Judas_Engine
{
	class JE_API Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);
		bool OnWindowClose(WindowClosedEvent& e);

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
}

