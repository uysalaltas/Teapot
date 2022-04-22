#pragma once

#ifdef TEA_PLATFORM_WINDOWS
	#ifdef TEA_BUILD_DLL
		#define	TEAPOT_API __declspec(dllexport)
	#else
		#define	TEAPOT_API __declspec(dllimport)
	#endif
#endif
