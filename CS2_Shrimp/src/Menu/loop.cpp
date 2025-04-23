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
void imp::menuloop()
{
    HWND hwnd = NULL;//绘制句柄
    WNDCLASSEXW wc = { sizeof(wc), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"imp_Class", nullptr };
    ::RegisterClassExW(&wc);
    if (sys::debug)
    {
        hwnd = ::CreateWindowW(wc.lpszClassName, L"Shrimp", WS_OVERLAPPEDWINDOW, GetSystemMetrics(0)/2-960, GetSystemMetrics(1)/2-540, 1920, 1024, nullptr, nullptr, wc.hInstance, nullptr);;
        // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return;
        }        
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
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return;
        }
        ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
        ::UpdateWindow(hwnd);
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    const float dpi_scale = DPIScaling::GetSystemScale();
    printf("系统缩放率: %.2f\n", dpi_scale);
    if (!dpi_scale)
        LOG_ERROR("获取DPI缩放比例失败!");
    FontSys::Fonts fonts;
    if (!FontSys::Initialize(fonts, dpi_scale)) {
        LOG_ERROR("部分字体无法加载或不存在!");
    }
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
    //main loop
    bool done = false;
    while (sys::loop)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                done=true;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (done)
            break;
        if (GetTickCount64() - time_上次刷新时间 > 1000)
        {
            sys::剩余时间--;
            帧数 = (uint32_t)ImGui::GetIO().Framerate;
            time_上次刷新时间 = GetTickCount64();
        }
        sys::dpi = DPIScaling::GetSystemScale();//整了两个小时还没正好的傻逼玩儿

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
        //if (show_demo_window)
        //    ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::SetNextWindowSize(ImVec2(800, 400));
        if (ImGui::Begin("sheimp.main", nullptr, 1 | 8 | 16)) {
            ImGui::Text("hallo e++");
            ImGui::Text("DPI:%.2f", sys::dpi);
            ImGui::End();
        }

        ImGui::GetForegroundDrawList()->AddRect(ImVec2(0, 0), ImVec2(50, 100), IM_COL32(255,255,255,255), 1.0f, ImDrawFlags_None, 1.0f);
        imp::addText(ImVec2(10.0f, 120.0f), IM_COL32(255, 255, 255, 255), sys::debug?L"调试模式":L"动态转换的中文");
        
        ImGui::Render();
        //const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };  // 完全透明背景
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
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
void InputHandler(HWND hWnd) {
    POINT p;
    GetCursorPos(&p);
    ScreenToClient(hWnd,&p);
    ImGui::GetIO().MousePos.x = p.x;
    ImGui::GetIO().MousePos.y = p.y;
}
BOOL UpdateWindowData(
    HWND hWnd,//游戏句柄
    HWND im_hWnd) {//绘制句柄
    
    static RECT old = { 0 };  // 静态变量保持状态
    RECT rect;
    POINT origin = {0,0};
    if (!IsWindow(hWnd)) {
        LOG_WARNING("无效窗口"); // 调试输出
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
