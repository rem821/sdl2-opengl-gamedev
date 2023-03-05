//
// Created by standa on 4.3.23.
//
#include <imgui/ImGuiLayer.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <Application.h>
#include <rendering/VulkanSwapChain.h>


namespace VulkanEngine {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {

    }

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::OnAttach() {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow *) Application::Get().GetWindow().GetNativeWindow(), true);

        Application &app = Application::Get();
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = app.GetDevice().GetInstance();
        init_info.PhysicalDevice = app.GetDevice().GetPhysicalDevice();
        init_info.Device = app.GetDevice().GetDevice();
        init_info.QueueFamily = app.GetDevice().FindPhysicalQueueFamilies().graphicsFamily;
        init_info.Queue = app.GetDevice().GraphicsQueue();
        init_info.PipelineCache = VK_NULL_HANDLE;
        init_info.DescriptorPool = app.GetDescriptorPool().GetPool();
        init_info.Subpass = 0;
        init_info.MinImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.ImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.Allocator = nullptr;
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, app.GetRenderer().GetSwapChainRenderPass());

        // Upload fonts
        VkCommandPool command_pool = app.GetDevice().GetCommandPool();
        VkCommandBuffer command_buffer;

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = app.GetDevice().GetCommandPool();
        allocInfo.commandBufferCount = 1;
        CORE_ASSERT(vkAllocateCommandBuffers(app.GetDevice().GetDevice(), &allocInfo, &command_buffer) == VK_SUCCESS, "Failed to allocate command buffer!")

        CORE_ASSERT(vkResetCommandPool(app.GetDevice().GetDevice(), command_pool, 0) == VK_SUCCESS, "Couldn't reset command pool")
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
        CORE_ASSERT(vkQueueSubmit(app.GetDevice().GraphicsQueue(), 1, &end_info, VK_NULL_HANDLE) == VK_SUCCESS, "Couldn't submit queue!")

        CORE_ASSERT(vkDeviceWaitIdle(app.GetDevice().GetDevice()) == VK_SUCCESS, "VkDeviceWaitIdle error!")
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnImGuiRender() {

        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void ImGuiLayer::Begin() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End(VkCommandBuffer commandBuffer) {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::Get();
        io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());;

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
}