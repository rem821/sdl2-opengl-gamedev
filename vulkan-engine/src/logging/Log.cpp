//
// Created by standa on 3.3.23.
//
#include "logging/Log.h"

namespace VulkanEngine {

    std::shared_ptr<spdlog::logger> Log::coreLogger_;
    std::shared_ptr<spdlog::logger> Log::clientLogger_;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        coreLogger_ = spdlog::stdout_color_mt("VULKAN_ENGINE");
        coreLogger_->set_level(spdlog::level::trace);

        clientLogger_ = spdlog::stdout_color_mt("APP");
        clientLogger_->set_level(spdlog::level::trace);
    }
}