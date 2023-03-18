#include "Application.h"

#include "Logging/Log.h"

namespace Judas_Engine
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		JE_CORE_INFO("{0}", e.ToString());
	}

	void Application::Run()
	{
		Judas_Engine::Log::Init();
		MouseButtonPressedEvent buttonPressed(1);
		JE_CORE_INFO(buttonPressed.ToString().c_str());
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}
