#pragma once

#include "Window.h"
#include "Layers/Layer.h"
#include "Layers/LayerStack.h"
#include "EventSystem/ApplicationEvent.h"

#include "ImGui/ImGuiLayer.h"

//TEMP
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		unsigned int m_VertexArray;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

