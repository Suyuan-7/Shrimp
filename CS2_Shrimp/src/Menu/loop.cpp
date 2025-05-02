#include "pch.hpp"
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
bool g_SwapChainOccluded = false;
UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
ID3D11ShaderResourceView* image_logo; ID3D11ShaderResourceView* image_esp;
int 当前选中 = 1;
bool 创建菜单导航按钮(ImDrawList* draw, ImVec2 pos, ImVec2 cpos, std::wstring title, ImTextureID ico, int selected_ID);
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL UpdateWindowData(HWND hWnd, HWND im_hWnd);
void LoadImageFile();
ImVec4 HexToImVec4(uint32_t color);
void 绘制文本(ImDrawList* draw, ImVec2 pos, ImU32 col, std::wstring text);
void 绘制文本_Fonts(ImDrawList* draw, ImFont* font, float font_size, ImVec2 pos, ImU32 col, std::wstring text);
void _RenderFrame();
const float 完全透明背景[4] = { 0, 0, 0, 0 };
const float 调试背景[4] = { 0.06f, 0.56f, 0.56f, 1.00f }; // 保持原始alpha值
/* mian */
void menuloop()
{
    /* 初始化ImGui上下文 */
    HWND hwnd;
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"imp Example", nullptr };
    ::RegisterClassExW(&wc);
    if (sys::debug)
    {
        hwnd = ::CreateWindowW(wc.lpszClassName, L"Shrimp", WS_OVERLAPPEDWINDOW, 500, 500, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
        sys::hWnd = hwnd;
    }
    else
    {
        hwnd = ::CreateWindowExW(NULL, wc.lpszClassName, L"Shrimp", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);
    }   
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        LOG_ERROR("Initialize Direct3D :%s")
        return;
    }  
    IMGUI_CHECKVERSION();
    if (!InitDirectX(sys::hWnd))
    {
        LOG_WARNING("Fail : InitDirectX!");
        return;
    }
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 2;
    config.RasterizerMultiply = sys::dpi;
    config.FontDataOwnedByAtlas = false;
    /* 初始化字体 */
    {
        conf::ExeFontDir = GetExecutableDirectory() + "\\fonts\\";
        conf::SysFontDir = GetSystemFontDirectory() + "\\";
        conf::Dir_Root = GetExecutableDirectory();
        conf::Dir_image = GetExecutableDirectory() + "\\image\\";
        conf::Dir_config = GetExecutableDirectory() + "\\config\\";
        const std::string fontPath = conf::SysFontDir + "msyh.ttc";
        const std::string boldFontPath = conf::SysFontDir + "msyhbd.ttc";
        const std::string quartzPath = conf::ExeFontDir + "Quartz.ttf";
        const std::string weaponiocPath = conf::ExeFontDir + "Weaponioc.ttf";
        const std::string stratum2Path = conf::ExeFontDir + "stratum2.ttf";
        sys::default_18 = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f * sys::dpi, &config, io.Fonts->GetGlyphRangesChineseFull());
        sys::default_22 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 22.0f * sys::dpi, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        sys::default_28 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 28.0f * sys::dpi);
        sys::default_48 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 48.0f * sys::dpi);
        sys::Quartz = io.Fonts->AddFontFromFileTTF(quartzPath.c_str(), 32.0f * sys::dpi);
        sys::Weaponioc = io.Fonts->AddFontFromFileTTF(weaponiocPath.c_str(), 64.0f * sys::dpi);
        sys::stratum2_18 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 18.0f * sys::dpi);
        sys::stratum2_32 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 32.0f * sys::dpi);
        sys::stratum2_64 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 64.0f * sys::dpi);
        io.FontDefault = sys::default_18;
        io.Fonts->Build();
    }    
    /* 初始化风格与颜色 */
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->ScaleAllSizes(sys::dpi);
        style->WindowPadding = ImVec2(0, 0);                   // 窗口填充
        style->WindowRounding = 5.0f * sys::dpi;               // 窗口圆角
        style->FramePadding = ImVec2(1 * sys::dpi, 1 * sys::dpi);         // 控件内边距
        style->FrameRounding = 5.0f * sys::dpi;                // 控件圆角
        style->ItemSpacing = ImVec2(3 * sys::dpi, 8 * sys::dpi);     // 项间距
        style->ItemInnerSpacing = ImVec2(8 * sys::dpi, 8 * sys::dpi); // 内部间距
        style->IndentSpacing = 4.0f * sys::dpi;                // 缩进宽度
        style->ScrollbarSize = 0.0f;                            // 滚动条宽度
        style->ScrollbarRounding = 0.0f;                        // 滚动条圆角
        style->GrabMinSize = 5.0f * sys::dpi;                  // 滑块最小尺寸
        style->GrabRounding = 3.0f * sys::dpi;                 // 滑块圆角
        style->WindowBorderSize = 0.0f;                         // 窗口边框
        style->FrameBorderSize = 0.0f;                          // 控件边框
        style->ChildRounding = 10.0f * sys::dpi;               // 子窗口圆角
        style->ButtonTextAlign = ImVec2(0.5f, 0.0f);            // 按钮文本对齐
        style->TabRounding = 1.0f * sys::dpi;                  // Tab圆角
        const ImVec4 silver_white = HexToImVec4(0xC0C0C0);
        const ImVec4 sky_blue = HexToImVec4(0x87CEEB);
        const ImVec4 gray = HexToImVec4(0x808080);
        style->Colors[ImGuiCol_WindowBg] = silver_white;        // 窗口背景
        style->Colors[ImGuiCol_ChildBg] = silver_white;         // 子窗口背景
        style->Colors[ImGuiCol_FrameBg] = silver_white;          // 控件背景
        style->Colors[ImGuiCol_Button] = HexToImVec4(2829099);  // 正常状态
        style->Colors[ImGuiCol_ButtonHovered] = HexToImVec4(14800384); // 悬停状态
        style->Colors[ImGuiCol_ButtonActive] = gray;            // 按下状态
        style->Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // 黑色文本
        style->Colors[ImGuiCol_CheckMark] = sky_blue;           // 开关开启背景
        style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // 开关滑块白色
        style->Colors[ImGuiCol_SliderGrabActive] = gray;         // 关闭状态滑块
    }
    if (!sys::debug) {
        ::SetWindowLongW(hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
        ::SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        ::ShowWindow(hwnd, SW_NORMAL);
    }
    else {
        ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    }
    ::UpdateWindow(hwnd);
    LoadImageFile();
    static ULONGLONG time_上次刷新时间 = 0;
    static uint32_t 帧数 = 0;
    bool done = false;
    MSG msg;
    while (sys::loop)
    {
        /* 消息处理 */
        {
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
        _RenderFrame();
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
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    if (sys::剩余时间 <= 0) {
        LOG_WARNING("用户已到期!");
    }
    LOG_WARNING("ImGui已卸载!");
    /* 资源清理 */
}
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
ImVec4 HexToImVec4(uint32_t color) {
    return ImVec4(
        ((color >> 16) & 0xFF) / 255.0f,  // 提取红色分量
        ((color >> 8) & 0xFF) / 255.0f,  // 提取绿色分量
        (color & 0xFF) / 255.0f,  // 提取蓝色分量
        1.0f                              // 固定Alpha为1.0f（不透明）
    );
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
/* 封装 */
void 绘制文本(ImDrawList* draw,ImVec2 pos,ImU32 col,std::wstring text)
{
    
    std::string utf8_str = WideToUTF8(text);
    ImGui::GetForegroundDrawList()->AddText(pos, col, utf8_str.c_str());
}
void 绘制文本_Fonts(ImDrawList* draw,ImFont* font, float font_size, ImVec2 pos, ImU32 col, std::wstring text)
{
    std::string utf8_str = WideToUTF8(text);
    draw->AddText(font, std::round(font_size * sys::dpi), pos, col, utf8_str.c_str());
}
/* 加载图片资源 */
void LoadImageFile()
{
    const std::string c_logo = conf::Dir_image + "logo.png";
    image_logo = LoadTextureFromFile(g_pd3dDevice, c_logo.c_str());

    const std::string c_esp = conf::Dir_image + "esp.png";
    image_esp = LoadTextureFromFile(g_pd3dDevice, c_esp.c_str());
}
/* 绘制菜单 */
void _RenderFrame()
{
    /* 开始渲染 */
    ImVec2 size = { 500.0f,700.0f };
    ImGui::SetNextWindowSize(size);

    if (ImGui::Begin("sheimp.main", nullptr, 1 | 8 | 16)) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();
        绘制文本(draw, ImVec2(pos.x + 75.0f, pos.y), IM_COL32(0, 0, 0, 255), L"HOME键显示菜单;END隐藏菜单");
        draw->AddRectFilledMultiColor(pos, ImVec2(pos.x + 70.0f, pos.y + 700.0f), IM_COL32(135, 206, 235, 150), IM_COL32(135, 206, 235, 150), IM_COL32(192, 192, 192, 255), IM_COL32(192, 192, 192, 255));
        draw->AddImage(reinterpret_cast<ImTextureID>(image_logo), pos, ImVec2(pos.x + 64.0f, pos.y + 64.0f));//大头像
        ImGui::SetCursorPos(ImVec2(500.0f - 32.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button + 1, IM_COL32(255, 0, 0, 255));
        ImGui::PushFont(sys::default_28);
        std::string off = WideToUTF8(L"×");
        if (ImGui::Button(off.c_str(), ImVec2(32.0f, 32.0f)))
        {
            static uintptr_t 上次退出时间;
            if (GetTickCount64() - 上次退出时间 < 3000){
                if (sys::ReadType) {
                    //VMMDLL_CloseAll();
                }
                sys::loop = 0;
            }
            else {
                LOG_WARNING("3秒内再按一次退出!");
                上次退出时间 = GetTickCount64();
            }
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(2);
        draw->AddLine(ImVec2(pos.x + 70.0f, pos.y + 32.0f), ImVec2(pos.x + 500.0f, pos.y + 32.0f), IM_COL32(232, 232, 232, 255), 1.0f);
        ImGui::SetCursorPosY(100);
        //if (创建菜单导航按钮(draw, pos, ImGui::GetCursorPos(), L"透视", reinterpret_cast<ImTextureID>(image_esp), 1))
        //{
        //    LOG_INFO("你选中了项目索引1");
        //}

        ImGui::End();
    }
    /* 结束渲染 */
}

bool 创建菜单导航按钮(ImDrawList* draw,ImVec2 pos, ImVec2 cpos,std::wstring title, ImTextureID ico, int selected_ID)
{
    bool ret = false;
    std::string u8_title = WideToUTF8(title);
    if (当前选中 == selected_ID)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(192, 192, 192, 255));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    }
    ImGui::SetCursorPosX(9);
    ImGui::PushID(u8_title.c_str());
    ret = ImGui::Button("##", ImVec2(52, 52));
    ImGui::PopID();
    draw->AddImage(ico, ImVec2(pos.x + cpos.x + 18, pos.x + cpos.x + 5), ImVec2(pos.x + cpos.x + 32 + 18, pos.y + cpos.y + 29));
    draw->AddText(sys::default_18, 16.0f * sys::dpi, ImVec2(pos.x + cpos.x + 20, pos.y + cpos.y + 35), ImColor(当前选中 == selected_ID ? 0x000000 : 0xCECECE),u8_title.c_str());
    ImGui::PopStyleColor(1);
    return ret;
}