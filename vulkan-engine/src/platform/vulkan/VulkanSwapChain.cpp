//
// Created by Stanislav Svědiroh on 13.06.2022.
//

#include <platform/vulkan/VulkanSwapChain.h>

namespace VulkanEngine {


    VulkanSwapChain::VulkanSwapChain(const VulkanDevice &engineDevice, VkExtent2D extent) : device_{engineDevice}, windowExtent_{extent} {
        Init();
    }

    VulkanSwapChain::VulkanSwapChain(const VulkanDevice &engineDevice, VkExtent2D extent, VulkanSwapChain *previous) : device_{engineDevice}, windowExtent_{extent},
                                                                                                                       oldSwapChain_{previous} {
        Init();

        oldSwapChain_ = nullptr;
    }

    void VulkanSwapChain::Init() {
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFrameBuffers();
        CreateSyncObjects();
    }


    VulkanSwapChain::~VulkanSwapChain() {
        for (auto imageView: swapChainImageViews_) {
            vkDestroyImageView(device_.GetDevice(), imageView, nullptr);
        }
        swapChainImageViews_.clear();

        if (swapChain_ != nullptr) {
            vkDestroySwapchainKHR(device_.GetDevice(), swapChain_, nullptr);
            swapChain_ = nullptr;
        }

        for (size_t i = 0; i < depthImages_.size(); i++) {
            vkDestroyImageView(device_.GetDevice(), depthImageViews_[i], nullptr);
            vkDestroyImage(device_.GetDevice(), depthImages_[i], nullptr);
            vkFreeMemory(device_.GetDevice(), depthImageMemory_[i], nullptr);
        }

        for (auto frameBuffer: swapChainFramebuffers_) {
            vkDestroyFramebuffer(device_.GetDevice(), frameBuffer, nullptr);
        }

        vkDestroyRenderPass(device_.GetDevice(), renderPass_, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device_.GetDevice(), renderFinishedSemaphores_[i], nullptr);
            vkDestroySemaphore(device_.GetDevice(), imageAvailableSemaphores_[i], nullptr);
            vkDestroyFence(device_.GetDevice(), inFlightFences_[i], nullptr);
        }
    }

    VkFormat VulkanSwapChain::FindDepthFormat() {
        return device_.FindSupportedFormat(
                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    VkResult VulkanSwapChain::AcquireNextImage(uint32_t *imageIndex) {
        vkWaitForFences(
                device_.GetDevice(),
                1,
                &inFlightFences_[currentFrame_],
                VK_TRUE,
                std::numeric_limits<uint64_t>::max()
        );

        VkResult result = vkAcquireNextImageKHR(
                device_.GetDevice(),
                swapChain_,
                std::numeric_limits<uint64_t>::max(),
                imageAvailableSemaphores_[currentFrame_],
                VK_NULL_HANDLE,
                imageIndex
        );
        return result;
    }

    VkResult VulkanSwapChain::SubmitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex) {
        if (imagesInFlight_[*imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(device_.GetDevice(), 1, &imagesInFlight_[*imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight_[*imageIndex] = inFlightFences_[currentFrame_];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores_[currentFrame_]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores_[currentFrame_]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(device_.GetDevice(), 1, &inFlightFences_[currentFrame_]);
        CORE_ASSERT(vkQueueSubmit(device_.GraphicsQueue(), 1, &submitInfo, inFlightFences_[currentFrame_]) == VK_SUCCESS, "Failed to submit draw command buffer!")

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain_};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(device_.PresentQueue(), &presentInfo);

        currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    void VulkanSwapChain::CreateSwapChain() {
        SwapChainSupportDetails swapChainSupport = device_.GetSwapChainSupport();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = device_.Surface();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = device_.FindPhysicalQueueFamilies();
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = oldSwapChain_ == nullptr ? VK_NULL_HANDLE : oldSwapChain_->swapChain_;

        CORE_ASSERT(vkCreateSwapchainKHR(device_.GetDevice(), &createInfo, nullptr, &swapChain_) == VK_SUCCESS, "Failed to create swap chain!")

        vkGetSwapchainImagesKHR(device_.GetDevice(), swapChain_, &imageCount, nullptr);
        swapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(device_.GetDevice(), swapChain_, &imageCount, swapChainImages_.data());

        swapChainImageFormat_ = surfaceFormat.format;
        swapChainExtent_ = extent;
    }

    void VulkanSwapChain::CreateImageViews() {
        swapChainImageViews_.resize(swapChainImages_.size());
        for (size_t i = 0; i < swapChainImages_.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = swapChainImages_[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = swapChainImageFormat_;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            CORE_ASSERT(vkCreateImageView(device_.GetDevice(), &viewInfo, nullptr, &swapChainImageViews_[i]) == VK_SUCCESS, "Failed to create texture image view!")
        }
    }

    void VulkanSwapChain::CreateDepthResources() {
        VkFormat depthFormat = FindDepthFormat();
        swapChainDepthFormat_ = depthFormat;
        VkExtent2D swapChainExt = GetSwapChainExtent();

        depthImages_.resize(ImageCount());
        depthImageMemory_.resize(ImageCount());
        depthImageViews_.resize(ImageCount());

        for (size_t i = 0; i < depthImages_.size(); i++) {
            VkImageCreateInfo imageInfo = {};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExt.width;
            imageInfo.extent.height = swapChainExt.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            device_.CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImages_[i], depthImageMemory_[i]);

            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = depthImages_[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            CORE_ASSERT(vkCreateImageView(device_.GetDevice(), &viewInfo, nullptr, &depthImageViews_[i]) == VK_SUCCESS, "Failed to create texture image view!")
        }
    }

    void VulkanSwapChain::CreateRenderPass() {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = FindDepthFormat();
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef = {};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = GetSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.srcStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstSubpass = 0;
        dependency.dstStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        CORE_ASSERT(vkCreateRenderPass(device_.GetDevice(), &renderPassInfo, nullptr, &renderPass_) == VK_SUCCESS, "Failed to create render pass!")
    }

    void VulkanSwapChain::CreateFrameBuffers() {
        swapChainFramebuffers_.resize(ImageCount());
        for (size_t i = 0; i < ImageCount(); i++) {
            std::array<VkImageView, 2> attachments = {swapChainImageViews_[i], depthImageViews_[i]};

            VkExtent2D swapChainExt = GetSwapChainExtent();
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass_;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExt.width;
            framebufferInfo.height = swapChainExt.height;
            framebufferInfo.layers = 1;

            CORE_ASSERT(vkCreateFramebuffer(device_.GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) == VK_SUCCESS, "Failed to create framebuffer!")
        }
    }

    void VulkanSwapChain::CreateSyncObjects() {
        imageAvailableSemaphores_.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores_.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences_.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight_.resize(ImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            CORE_ASSERT(vkCreateSemaphore(device_.GetDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores_[i]) ==
                        VK_SUCCESS &&
                        vkCreateSemaphore(device_.GetDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores_[i]) ==
                        VK_SUCCESS &&
                        vkCreateFence(device_.GetDevice(), &fenceInfo, nullptr, &inFlightFences_[i]) == VK_SUCCESS, "Failed to create synchronization objects for a frame!")
        }
    }

    VkSurfaceFormatKHR
    VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR
    VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode: availablePresentModes) {
//            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
//                CORE_INFO("Present mode: Mailbox");
//                return availablePresentMode;
//            }

            if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                CORE_INFO("Present mode: Immediate");
                return availablePresentMode;
            }
        }

        CORE_INFO("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = windowExtent_;
            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                           std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }
}