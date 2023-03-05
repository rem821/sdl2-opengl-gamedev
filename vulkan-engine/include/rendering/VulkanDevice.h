//
// Created by Stanislav Svědiroh on 04.03.2023.
//
#pragma once

#include <precompiled_headers/PCH.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace VulkanEngine {

    struct QueueFamilyIndices {
        uint32_t graphicsFamily{};
        uint32_t presentFamily{};
        bool graphicsFamilyHasValue = false;
        bool presentFamilyHasValue = false;

        [[nodiscard]] bool isComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanDevice {
    public:
        explicit VulkanDevice(GLFWwindow* window, bool enableValidationLayers);

        ~VulkanDevice();

        VulkanDevice(const VulkanDevice &) = delete;
        VulkanDevice operator=(const VulkanDevice &) = delete;
        VulkanDevice(VulkanDevice &&) = delete;
        VulkanDevice &operator=(VulkanDevice &&) = delete;

        [[nodiscard]] VkCommandPool GetCommandPool() const { return commandPool_; }

        [[nodiscard]] VkInstance GetInstance() const{ return instance_; }

        [[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice_; }

        [[nodiscard]] VkDevice GetDevice() const { return device_; }

        [[nodiscard]] VkSurfaceKHR Surface() const { return surface_; }

        [[nodiscard]] VkQueue GraphicsQueue() const { return graphicsQueue_; }

        [[nodiscard]] VkQueue PresentQueue() const { return presentQueue_; }

        [[nodiscard]] SwapChainSupportDetails GetSwapChainSupport() const;

        [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        [[nodiscard]] QueueFamilyIndices FindPhysicalQueueFamilies() const { return FindQueueFamilies(physicalDevice_); }

        [[nodiscard]] VkFormat FindSupportedFormat(
                const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

        void CreateImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory) const;

    private:
        void CreateInstance();
        void CreateSurface(GLFWwindow* window);
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();

        bool CheckValidationLayerSupport();
        [[nodiscard]] QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device) const;

        [[nodiscard]] std::vector<const char *> GetRequiredExtensions() const;

        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        void SetupDebugMessenger() const;

        bool enableValidationLayers_;

        const std::string appName_ = "VulkanEngineSandbox";
        const std::string engineName_ = "VulkanEngine";
        const std::vector<const char *> validationLayers_ = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkInstance instance_{};
        VkSurfaceKHR surface_{};
        VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
        VkDevice device_{};
        VkCommandPool commandPool_{};

        VkQueue graphicsQueue_{};
        VkQueue presentQueue_{};

        VkPhysicalDeviceProperties physicalDeviceProperties_{};

        VkDebugUtilsMessengerEXT debugMessenger_{};
    };
}
