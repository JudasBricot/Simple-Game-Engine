#pragma once

#include <iostream>
#include "Window.h"

#include "EventSystem/ApplicationEvent.h"


namespace Judas_Engine
{
	class Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);

		void Run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
}

