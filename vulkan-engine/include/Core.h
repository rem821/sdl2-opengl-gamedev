#pragma once

#ifdef ENABLE_ASSERTS
    #define CORE_ASSERT(x, ...) { if(!(x)) { CORE_ERROR("Assertion Failed: {}", __VA_ARGS__); __builtin_trap(); } }
    #define CLIENT_ASSERT(x, ...) { if(!(x)) { CLIENT_ERROR("Assertion Failed: {}", __VA_ARGS__); __builtin_trap(); } }
#else
    #define CORE_ASSERT(x, ...)
    #define CLIENT_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
