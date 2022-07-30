#pragma once

#ifdef TEA_PLATFORM_WINDOWS
	#define	TEAPOT_API
	// #ifdef TEA_BUILD_DLL
	// 	#define	TEAPOT_API __declspec(dllexport)
	// #else
	// 	#define	TEAPOT_API __declspec(dllimport)
	// #endif
#elif TEA_PLATFORM_LINUX
	#define TEAPOT_API __attribute__((visibility("default")))
#endif
