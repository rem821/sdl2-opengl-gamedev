#pragma once

#ifdef PLATFORM_WINDOWS
    #ifdef BUILD_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
#elif PLATFORM_LINUX
    #ifdef BUILD_DLL
        #define ENGINE_API
    #else
        #define ENGINE_API
    #endif
#else
    #error "This platform is not supported!"
#endif

#define BIT(x) (1 << x)