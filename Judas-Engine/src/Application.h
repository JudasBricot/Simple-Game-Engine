#pragma once

#include "Window.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "EventSystem/ApplicationEvent.h"

#include "Core/Timestep.h"

#include "ImGui/ImGuiLayer.h"

namespace Judas_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);

		void PushOverlay(Layer* overlay);
		void PushLayer(Layer* layer);

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizedEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

