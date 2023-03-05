//
// Created by standa on 5.3.23.
//
#include <platform/vulkan/VulkanContext.h>
#include <imgui_impl_vulkan.h>

namespace VulkanEngine {

    VulkanContext::VulkanContext(const Window &window) : window_(window) {
        CORE_INFO("Creating Vulkan Context!");
        vulkanDevice_ = std::make_unique<VulkanDevice>((GLFWwindow *) window_.GetNativeWindow(), true);
        vulkanRenderer_ = std::make_unique<VulkanRenderer>(window, *vulkanDevice_);
        globalPool_ = VulkanDescriptorPool::Builder(*vulkanDevice_)
                .SetMaxSets(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
                .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VulkanSwapChain::MAX_FRAMES_IN_FLIGHT)
                .Build();
    }

    void VulkanContext::Init() {

    }

    void VulkanContext::InitImGuiVulkan() {
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = vulkanDevice_->GetInstance();
        init_info.PhysicalDevice = vulkanDevice_->GetPhysicalDevice();
        init_info.Device = vulkanDevice_->GetDevice();
        init_info.QueueFamily = vulkanDevice_->FindPhysicalQueueFamilies().graphicsFamily;
        init_info.Queue = vulkanDevice_->GraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = globalPool_->GetPool();
        init_info.Subpass = 0;
        init_info.MinImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, vulkanRenderer_->GetSwapChainRenderPass());

        // Upload fonts
        VkCommandPool command_pool = vulkanDevice_->GetCommandPool();
        VkCommandBuffer command_buffer;

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = vulkanDevice_->GetCommandPool();
        allocInfo.commandBufferCount = 1;
        CORE_ASSERT(vkAllocateCommandBuffers(vulkanDevice_->GetDevice(), &allocInfo, &command_buffer) == VK_SUCCESS, "Failed to allocate command buffer!")

        CORE_ASSERT(vkResetCommandPool(vulkanDevice_->GetDevice(), command_pool, 0) == VK_SUCCESS, "Couldn't reset command pool")
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        CORE_ASSERT(vkBeginCommandBuffer(command_buffer, &begin_info) == VK_SUCCESS, "Couldn't begin command buffer")

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        CORE_ASSERT(vkEndCommandBuffer(command_buffer) == VK_SUCCESS, "Couldn't end command buffer")
        CORE_ASSERT(vkQueueSubmit(vulkanDevice_->GraphicsQueue(), 1, &end_info, VK_NULL_HANDLE) == VK_SUCCESS, "Couldn't submit queue!")

        CORE_ASSERT(vkDeviceWaitIdle(vulkanDevice_->GetDevice()) == VK_SUCCESS, "VkDeviceWaitIdle error!")
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    VkCommandBuffer VulkanContext::BeginFrame() {
        auto commandBuffer = vulkanRenderer_->BeginFrame();
        vulkanRenderer_->BeginSwapChainRenderPass(commandBuffer);

        return commandBuffer;
    }

    void VulkanContext::EndFrame(VkCommandBuffer commandBuffer) {
        vulkanRenderer_->EndSwapChainRenderPass(commandBuffer);
        vulkanRenderer_->EndFrame();
    }
}