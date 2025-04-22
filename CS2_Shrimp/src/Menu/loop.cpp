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
void imp::imgui()
{
    HWND hwnd = NULL;
    WNDCLASSEXW wc = { sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"imp_Class", nullptr };
    ::RegisterClassExW(&wc);
    if (sys::debug)
    {
        hwnd = ::CreateWindowW(wc.lpszClassName, L"Shrimp", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1024, nullptr, nullptr, wc.hInstance, nullptr);;
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hwnd);
        sys::hWnd = hwnd;
    }
    else
    {
        hwnd = ::CreateWindowExW(
            WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW,
            wc.lpszClassName,
            L"Shrimp",
            WS_POPUP,
            0, 0,
            GetSystemMetrics(SM_CXSCREEN),
            GetSystemMetrics(SM_CYSCREEN),
            nullptr, nullptr, wc.hInstance, nullptr
        );
        ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hwnd);
    }
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return ;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // 1. DPI适配
    const float dpi_scale = GetSystemDPI();
    io.FontGlobalScale = dpi_scale;
    // 2. 字体加载（带多路径回退）
    constexpr const char* font_paths[] = {
        "C:/Windows/Fonts/simhei.ttf",         // Windows系统路径
        "fonts/simhei.ttf",                   // 项目相对路径
    };
    bool font_loaded = false;
    for (const auto& path : font_paths) {
        if (std::ifstream(path).good()) {
            if (io.Fonts->AddFontFromFileTTF(
                path,
                18.0f * dpi_scale,  // 按DPI缩放字号
                nullptr,
                io.Fonts->GetGlyphRangesChineseFull()))
            {
                font_loaded = true;
                break;
            }
        }
    }
    if (!font_loaded) {
        LOG_WARNING("警告：使用默认字体替代中文字体");
        io.Fonts->AddFontDefault();
    }
    // 3. 输入控制配置
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
    // 4. 样式配置（按DPI缩放）
    ImGuiStyle& style = ImGui::GetStyle();
    const auto scale_style = [&](auto& value) { value *= dpi_scale; };
    style.WindowPadding = ImVec2(0, 0);
    style.WindowRounding = 5.0f * dpi_scale;
    style.FramePadding = ImVec2(1* dpi_scale, 1* dpi_scale);
    style.FrameRounding = 5.0f * dpi_scale;
    style.ItemSpacing = ImVec2(3 * dpi_scale, 8 * dpi_scale) ;
    style.ItemInnerSpacing = ImVec2(8 * dpi_scale, 8 * dpi_scale) ;
    style.IndentSpacing = 4.0f * dpi_scale;
    style.ScrollbarRounding = 0;
    style.ScrollbarSize = 0;
    style.GrabRounding = 3.0f * dpi_scale;
    style.GrabMinSize = 5.0f * dpi_scale;
    style.WindowBorderSize = 0;
    style.FrameBorderSize = 0;
    style.ChildRounding = 10.0f * dpi_scale;
    style.ButtonTextAlign = ImVec2(0.5f, 0);
    style.TabRounding = 1.0f * dpi_scale;
    // 5. 构建字体集
    io.Fonts->Build();

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    static ULONGLONG time_上次刷新时间 = 0;
    static uint32_t 帧数 = 0;
    bool done = false;
    
    //main loop
    while (sys::剩余时间>0)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;
        if (GetTickCount64() - time_上次刷新时间 > 1000)
        {
            sys::剩余时间--;
            帧数 = (uint32_t)ImGui::GetIO().Framerate;
            time_上次刷新时间 = GetTickCount64();
        }
        sys::dpi = GetSystemDPI();
        if (sys::debug || sys::ReadType==false) {
            sys::ScreenX = ImGui::GetIO().DisplaySize.x * sys::dpi / 2;
            sys::ScreenY = ImGui::GetIO().DisplaySize.y * sys::dpi / 2;
        }
        else {
            sys::ScreenX = sys::SizeX * 2;
            sys::ScreenY = sys::SizeY * 2;
        }
        if (UpdateWindowData(sys::hWnd,hwnd)==false) {
            break;
        }
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool show_window = true;
        if (show_window) {  // 先检查状态
            if (ImGui::Begin("Conditional Window", &show_window)) {
                ImGui::Text("内容仅在窗口打开时渲染");
            }
            ImGui::End();
        }


        ImGui::GetForegroundDrawList()->AddRect(ImVec2(0, 0), ImVec2(50, 100), IM_COL32(255,255,255,255), 1.0f, ImDrawFlags_None, 1.0f);
        绘制文本(ImVec2(10.0f, 120.0f), IM_COL32(255, 255, 255, 255), L"动态转换的中文");
        
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };  // 完全透明背景
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
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
// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
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
        MARGINS margins = { -1, -1, -1, -1 };
        DwmExtendFrameIntoClientArea(hWnd, &margins);
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
void InputHandler(HWND hWnd) {
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd,&p);
    ImGui::GetIO().MousePos.x = p.x;
    ImGui::GetIO().MousePos.y = p.y;
}
BOOL UpdateWindowData(HWND hWnd,HWND im_hWnd) {
    static RECT old = { 0 };  // 静态变量保持状态
    RECT rect;
    // 检查窗口有效性
    if (!IsWindow(hWnd)) {
        LOG_WARNING("无效窗口"); // 调试输出
        return false;
    }
    CreateRenderTarget(); // 假设已实现的创建渲染目标函数
    // 获取当前窗口扩展样式
    LONG style = GetWindowLongW(im_hWnd, GWL_EXSTYLE);
    if (ImGui::GetIO().WantCaptureMouse)
    {
        // ImGui需要鼠标输入：移除WS_EX_LAYERED（允许鼠标交互）
        if (style & WS_EX_LAYERED)
        {
            SetWindowLongW(im_hWnd, GWL_EXSTYLE, style & ~WS_EX_LAYERED);
        }
    }
    else
    {
        // ImGui不需要鼠标输入：添加WS_EX_LAYERED（恢复透明穿透）
        if (!(style & WS_EX_LAYERED))
        {
            SetWindowLongW(im_hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
        }
    }
    return 1;
}
void LoadFonts() {
        //字体目录 ＝ 取特定目录(4) ＋ “msyh.ttc”
        //Sys._默认字体_18 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录, 18, , ig.GetFontAtlas().GetGlyphRangesChineseFull())
        //字体目录 ＝ 取特定目录(4) ＋ “msyhbd.ttc”
        //Sys._默认字体_22 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录, 22, , ig.GetFontAtlas().GetGlyphRangesChineseFull())
        //Sys._默认字体_28 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录, 28, , )
        //Sys._默认字体_48 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录, 48, , )
        //字体目录 ＝ 取运行目录() ＋ “\fonts\”
        //Sys._数字字体2_18 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “stratum2.ttf”, 18, , )
        //Sys._数字字体2_32 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “stratum2.ttf”, 32, , )
        //Sys._数字字体2_64 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “stratum2.ttf”, 64, , )
        //Sys._数字字体_32 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “Quartz.ttf”, 32, , )
        //Sys._软件图标_28 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “tga.ttf”, 28, , )
        //Sys._武器字体 ＝ ig.GetFontAtlas().AddFontFromFileTTF(字体目录 ＋ “Weaponioc.ttf”, 64, , )

}
void 绘制文本(ImVec2 pos,ImU32 col,std::wstring chinese_text)
{
    std::string utf8_str = WideToUTF8(chinese_text);
    ImGui::GetForegroundDrawList()->AddText(pos, col, utf8_str.c_str());
}