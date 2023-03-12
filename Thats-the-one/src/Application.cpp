#include "Application.h"

#include "EventSystem/ApplicationEvent.h"

Application::Application()
{
	std::string name = "My game";

	m_Window = std::unique_ptr<Window>(Window::Create({ 640, 480, name.c_str()}));
}

Application::~Application()
{
}

void Application::Run()
{
	MouseButtonPressedEvent buttonPressed(1);
	std::cout << buttonPressed.ToString() << std::endl;
	while (m_Running)
	{
		m_Window->Update();
	}
}
