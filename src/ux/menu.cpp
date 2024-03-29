#include "menu.h"

void Menu::render(State& state) {
    create_ctx();
    KeyHandler::install_hook();

    constexpr float bg[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    while (!state.stop.stop_requested()) {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) state.stop.request_stop();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("limits", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

        for (auto& cb : state.callbacks) {
            ImGui::PushItemWidth(25.0f);
            ImGui::InputInt(("##" + cb->name).c_str(), &cb->keybind, 0, 0, ImGuiInputTextFlags_CharsHexadecimal);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::Checkbox(cb->name.c_str(), &cb->enabled);
            ImGui::SameLine();

            ImGui::TextDisabled("[?]");
            if (ImGui::BeginItemTooltip()) {
                ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20.0f);
                ImGui::TextUnformatted(cb->tooltip.c_str());
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::Render();
        devicectx->OMSetRenderTargets(1, &rendertargetview, nullptr);
        devicectx->ClearRenderTargetView(rendertargetview, bg);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapchain->Present(1, 0);
    }

    KeyHandler::uninstall_hook();
    destroy_ctx();
}

void Menu::create_ctx() {
    create_window();
    create_device();
    create_rendertarget();

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    auto* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImColor(22, 22, 22);
    colors[ImGuiCol_FrameBg] = ImColor(55, 55, 55);

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(device, devicectx);
}

void Menu::destroy_ctx() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    cleanup_rendertarget();
    cleanup_device();
    cleanup_window();
}

void Menu::create_window() {
    ZeroMemory(&wndclass, sizeof(wndclass));
    wndclass.style = CS_CLASSDC;
    wndclass.lpfnWndProc = wnd_proc;
    wndclass.hInstance = GetModuleHandleA(nullptr);
    wndclass.lpszClassName = "dilemna";
    RegisterClass(&wndclass);

    hwnd = CreateWindow("dilemna", "dilemna", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 100, 100, 300, 200, nullptr, nullptr, nullptr, nullptr);
}

void Menu::cleanup_window() {
    DestroyWindow(hwnd);
}

void Menu::create_device() {
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL features[] = {D3D_FEATURE_LEVEL_11_0 };
    D3D11CreateDeviceAndSwapChain(nullptr,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  nullptr,
                                  0,
                                  features,
                                  1,
                                  D3D11_SDK_VERSION,
                                  &sd,
                                  &swapchain,
                                  &device,
                                  nullptr,
                                  &devicectx);
}

void Menu::create_rendertarget() {
    ID3D11Texture2D* backbuf;
    swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuf));
    device->CreateRenderTargetView(backbuf, nullptr, &rendertargetview);
    backbuf->Release();
}

void Menu::cleanup_device() {
    cleanup_rendertarget();
    swapchain->Release();
    devicectx->Release();
    device->Release();
}

void Menu::cleanup_rendertarget() {
    if (rendertargetview) rendertargetview->Release();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WINAPI Menu::wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wparam, lparam);
}