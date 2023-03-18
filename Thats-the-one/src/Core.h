#pragma once

#ifdef JE_PLATFORM_WINDOWS
	#ifdef JE_BUILD_DLL
		#define JUDAS_ENGINE_API declspec(dllexport)
	#else
		#define JUDAS_ENGINE_API declspec(dllimport)

	#endif // JE_BUILD_DLL
#else
	#error Judas Engine only support Windows!
#endif // JE_PLATFORM_WINDOWS

#ifdef JE_ENABLE_ASSERT
	#define JE_ASSERT(x,...) { if(!(x)) { JE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define JE_CORE_ASSERT(x,...) { if(!(x)) { JE_CORE_ASSERT("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define JE_ASSERT(x,...)
	#define JE_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1 << x);
