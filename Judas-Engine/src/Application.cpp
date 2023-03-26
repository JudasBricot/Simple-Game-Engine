#include "Application.h"
#include "Logging/Log.h"

#include <glad/glad.h>

namespace Judas_Engine
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application::Application()
	{
		Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		unsigned int id;
		glCreateBuffers(1, &id);
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));
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
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::Run()
	{
		MouseButtonPressedEvent buttonPressed(1);
		JE_CORE_INFO(buttonPressed.ToString().c_str());
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	Application* CreateApplication()
	{
		return new Application();
	}
}
