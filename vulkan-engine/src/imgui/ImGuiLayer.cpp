//
// Created by standa on 4.3.23.
//
#include <imgui/ImGuiLayer.h>
#include <imgui.h>
#include <platform/vulkan/imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <Application.h>
#include <rendering/VulkanSwapChain.h>


namespace VulkanEngine {

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {

    }

    ImGuiLayer::~ImGuiLayer() {

    }

    // VulkanDevice &engineDevice, VulkanRenderer &renderer, GLFWwindow *window
    void ImGuiLayer::OnAttach() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO &io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = VE_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = VE_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = VE_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = VE_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = VE_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = VE_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = VE_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = VE_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = VE_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = VE_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = VE_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = VE_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = VE_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = VE_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = VE_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = VE_KEY_A;
        io.KeyMap[ImGuiKey_C] = VE_KEY_C;
        io.KeyMap[ImGuiKey_V] = VE_KEY_V;
        io.KeyMap[ImGuiKey_X] = VE_KEY_X;
        io.KeyMap[ImGuiKey_Y] = VE_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = VE_KEY_Z;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow *) Application::Get().GetWindow().GetNativeWindow(), false);

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

    }

    void ImGuiLayer::OnUpdate(VkCommandBuffer commandBuffer) {
        ImGuiIO &io = ImGui::GetIO();
        Application &app = Application::Get();
        io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float) app.GetWindow().GetHeight());;

        auto time = (float) glfwGetTime();
        io.DeltaTime = time_ > 0.0 ? (time - time_) : (1.0f / 60.0f);
        time_ = time;


        ImGui_ImplVulkan_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    void ImGuiLayer::OnEvent(Event &event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.MouseWheel += e.GetYOffset();
        io.MouseWheelH += e.GetXOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[VE_KEY_LEFT_CONTROL] || io.KeysDown[VE_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[VE_KEY_LEFT_SHIFT] || io.KeysDown[VE_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[VE_KEY_LEFT_ALT] || io.KeysDown[VE_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[VE_KEY_LEFT_SUPER] || io.KeysDown[VE_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        int keyCode = e.GetKeyCode();

        if (keyCode > 0 && keyCode < 0x10000)
            io.AddInputCharacter((unsigned short) keyCode);

        return false;
    }

    bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent &e) {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float) e.GetWidth(), (float) e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        return false;
    }
}