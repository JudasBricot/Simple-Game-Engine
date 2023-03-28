#include "Application.h"

#include "Core.h"
#include "Logging/Log.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		JE_CORE_ASSERT(!s_Instance, "Another application has already be created !")
		s_Instance = this;

		Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		unsigned int id;
		glCreateBuffers(1, &id);
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		JE_CORE_INFO("{0}", e.ToString());

		for (auto it = m_LayerStack.begin(); it != m_LayerStack.end();)
		{
			(*it)->OnEvent(e);
			it++;
			if (e.handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		MouseButtonPressedEvent buttonPressed(1);
		JE_CORE_INFO(buttonPressed.ToString().c_str());
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

	Application* CreateApplication()
	{
		return new Application();
	}
}
