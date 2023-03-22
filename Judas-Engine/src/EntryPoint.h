#pragma once

#include "Application.h"

#ifdef JE_PLATFORM_WINDOWS

extern Judas_Engine::Application* Judas_Engine::CreateApplication();

int main()
{
	Judas_Engine::Application* app = Judas_Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif