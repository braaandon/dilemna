#pragma once

#include <d3d11.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "state.h"
#include "keyhandler.h"

class Menu {
public:
    void render(State& state);

private:
    HWND hwnd;
    WNDCLASSA wndclass;
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* devicectx = nullptr;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11RenderTargetView* rendertargetview = nullptr;

    static int resize_width;
    static int resize_height;

    static LRESULT WINAPI wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    void create_ctx();
    void destroy_ctx();

    void create_window();
    void cleanup_window();

    void create_device();
    void create_rendertarget();

    void cleanup_device();
    void cleanup_rendertarget();
};
