#include "pch.hpp"

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void menuloop()
{
    /* 绘制句柄 */
    HWND hwnd = NULL;
    WNDCLASSEXW wc = { sizeof(wc), sys::debug ? CS_CLASSDC : CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"imp_Class", nullptr };
    ::RegisterClassExW(&wc);
    if (sys::debug)
    {
        hwnd = ::CreateWindowW(wc.lpszClassName, L"Shrimp", WS_OVERLAPPEDWINDOW, GetSystemMetrics(0) / 2 - 960, GetSystemMetrics(1) / 2 - 540, 1920, 1024, nullptr, nullptr, wc.hInstance, nullptr);
        sys::hWnd = hwnd;
    }
    else
    {
        hwnd = ::CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, wc.lpszClassName, L"Shrimp", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);
    }   
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        LOG_ERROR("Initialize Direct3D :%s")
        return;
    }
    if (!sys::debug) {
        ::SetWindowLongW(hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    }
    else {
        ::ShowWindow(hwnd, SW_NORMAL);
    }
    ::UpdateWindow(hwnd);   
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 2;
    config.RasterizerMultiply = std::clamp(sys::dpi, 1.0f, 2.0f);
    config.FontDataOwnedByAtlas = false;
    /* 初始化字体 */
    sys::ExeFontDir = GetExecutableDirectory() + "\\fonts\\";
    sys::SysFontDir = GetSystemFontDirectory() + "\\";
    const std::string fontPath = sys::SysFontDir + "msyh.ttc";
    const std::string boldFontPath = sys::SysFontDir + "msyhbd.ttc";
    const std::string quartzPath = sys::ExeFontDir + "Quartz.ttf";
    const std::string weaponiocPath = sys::ExeFontDir + "Weaponioc.ttf";
    const std::string stratum2Path = sys::ExeFontDir + "stratum2.ttf";
    sys::default_18 = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f * sys::dpi, &config, io.Fonts->GetGlyphRangesChineseFull());
    sys::default_22 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 22.0f * sys::dpi, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    sys::default_28 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 28.0f * sys::dpi);
    sys::default_48 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 48.0f * sys::dpi);
    sys::Quartz = io.Fonts->AddFontFromFileTTF(quartzPath.c_str(), 32.0f * sys::dpi);
    sys::Weaponioc = io.Fonts->AddFontFromFileTTF(weaponiocPath.c_str(), 64.0f * sys::dpi);
    sys::stratum2_18 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 18.0f * sys::dpi);
    sys::stratum2_32 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 32.0f * sys::dpi);
    sys::stratum2_64 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 64.0f * sys::dpi);
    io.Fonts->Build();
    ImGuiStyle* style = &ImGui::GetStyle();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const float 完全透明背景[4] = { 0, 0, 0, 0 };
    const float 调试背景[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    static ULONGLONG time_上次刷新时间 = 0;
    static uint32_t 帧数 = 0;
    bool done = false;
    bool demo = true;
    while (sys::loop)
    {
        /* 消息处理 */
        {
            MSG msg;
            while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    done = true;
            }
            if (done)
                break;
        }
        //dpi缩放计算
        {
            if (GetTickCount64() - time_上次刷新时间 > 1000)
            {
                sys::剩余时间--;
                帧数 = (uint32_t)ImGui::GetIO().Framerate;
                time_上次刷新时间 = GetTickCount64();
            }
            if (sys::debug || sys::ReadType == false) {
                sys::ScreenX = ImGui::GetIO().DisplaySize.x * sys::dpi / 2;
                sys::ScreenY = ImGui::GetIO().DisplaySize.y * sys::dpi / 2;
            }
            else {
                sys::ScreenX = sys::SizeX * 2;
                sys::ScreenY = sys::SizeY * 2;
            }
            if (UpdateWindowData(sys::hWnd, hwnd) == false) {
                break;
            }
            if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
            {
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
                g_ResizeWidth = g_ResizeHeight = 0;
                CreateRenderTarget();
            }
        }
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&demo);
        /* 渲染菜单 */
        menu();
        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, sys::debug ? 调试背景 : 完全透明背景);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        HRESULT hr = g_pSwapChain->Present(sys::垂直同步 ? 1 : 0, 0);
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    if (sys::剩余时间 <= 0) {
        LOG_WARNING("用户已到期!");
    }
    LOG_WARNING("ImGui已卸载!");
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}
void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}
void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}
void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_CREATE:
        if (const auto dwm = LoadLibrary("dwmapi.dll")) {
            using DwmExtendFrameFn = HRESULT(WINAPI*)(HWND, const MARGINS*);
            if (const auto func = reinterpret_cast<DwmExtendFrameFn>(
                GetProcAddress(dwm, "DwmExtendFrameIntoClientArea"))) {
                MARGINS margins = { -1 };
                func(hWnd, &margins);
            }
            FreeLibrary(dwm);
        }
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
BOOL UpdateWindowData(HWND hWnd,HWND im_hWnd) {
    
    static RECT old = { 0 };
    RECT rect;
    POINT origin = {0,0};
    if (!IsWindow(hWnd)) {
        LOG_WARNING("无效窗口");
        return false;
    }
    if (sys::debug == false) {
        GetClientRect(hWnd, &rect);
        ClientToScreen(hWnd, &origin);
        SetWindowPos(im_hWnd,
            HWND_TOPMOST,
            origin.x,
            origin.y,
            rect.right - rect.left,
            rect.bottom - rect.top,
            SWP_SHOWWINDOW);
    }
    return 1;
}
void imp::addText(ImVec2 pos,ImU32 col,std::wstring chinese_text)
{
    std::string utf8_str = WideToUTF8(chinese_text);
    ImGui::GetForegroundDrawList()->AddText(pos, col, utf8_str.c_str());
}
/* 绘制菜单 */
void menu()
{
    ImGui::SetNextWindowSize(ImVec2(800, 400));
    if (ImGui::Begin("sheimp.main", nullptr, 1 | 8 | 16)) {
        ImGui::Text("hallo e++");
        ImGui::End();       
    }
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(0, 0), ImVec2(50, 100), IM_COL32(255, 255, 255, 255), 1.0f, ImDrawFlags_None, 1.0f);
    imp::addText(ImVec2(10.0f, 120.0f), IM_COL32(255, 255, 255, 255), sys::debug ? L"调试模式" : L"动态转换的中文");
}
/* 绘制玩家 */
void objDraw()
{

}