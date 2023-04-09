#pragma once

#ifdef JE_PLATFORM_WINDOWS
#if JE_DYNAMIC_LINKING
	#ifdef JE_BUILD_DLL
		#define JE_API __declspec(dllexport)
	#else
		#define JE_API __declspec(dllimport)

	#endif // JE_BUILD_DLL
#else
	#define JE_API
#endif
#else
	#error Judas Engine only support Windows!
#endif // JE_PLATFORM_WINDOWS

#ifdef JE_ENABLE_ASSERT
	#define JE_ASSERT(x,...) { if(!(x)) { JE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define JE_CORE_ASSERT(x,...) { if(!(x)) { JE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define JE_ASSERT(x,...)
	#define JE_CORE_ASSERT(x,...)
#endif

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x);
