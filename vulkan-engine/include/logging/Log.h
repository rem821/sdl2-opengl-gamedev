//
// Created by standa on 3.3.23.
//
#pragma once

#include "../Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace VulkanEngine {
    class ENGINE_API Log {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger_; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger_; }

    private:
        static std::shared_ptr<spdlog::logger> coreLogger_;
        static std::shared_ptr<spdlog::logger> clientLogger_;
    };
}

// Core log macros
#define CORE_CRITICAL(...)  ::VulkanEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CORE_ERROR(...)     ::VulkanEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_WARN(...)      ::VulkanEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_INFO(...)      ::VulkanEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_TRACE(...)     ::VulkanEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

// client log macros
#define CLIENT_CRITICAL(...)  ::VulkanEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CLIENT_ERROR(...)     ::VulkanEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CLIENT_WARN(...)      ::VulkanEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CLIENT_INFO(...)      ::VulkanEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CLIENT_TRACE(...)     ::VulkanEngine::Log::GetClientLogger()->trace(__VA_ARGS__)