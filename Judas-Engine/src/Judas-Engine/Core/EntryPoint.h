#pragma once

#include "Application.h"
#include "Judas-Engine/Debug/Profiler.h"

#ifdef JE_PLATFORM_WINDOWS

extern Judas_Engine::Application* Judas_Engine::CreateApplication();

int main()
{
	{
		JE_PROFILE_SESSION_BEGIN("startup.json")
		Judas_Engine::Application* app = Judas_Engine::CreateApplication();
		JE_PROFILE_SESSION_END

		JE_PROFILE_SESSION_BEGIN("runtime.json")
		app->Run();
		JE_PROFILE_SESSION_END

		JE_PROFILE_SESSION_BEGIN("cleanup.json")
		delete app;
		JE_PROFILE_SESSION_END
	}
}

#endif