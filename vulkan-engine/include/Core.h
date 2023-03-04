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

#ifdef ENABLE_ASSERTS
    #define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __builtin_trap(); } }
    #define CLIENT_ASSERT(x, ...) { if(!(x)) { CLIENT_ERROR("Assertion Failed: {}", __VA_ARGS__); __builtin_trap(); } }
#else
    #define CORE_ASSERT(x, ...)
    #define CLIENT_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
