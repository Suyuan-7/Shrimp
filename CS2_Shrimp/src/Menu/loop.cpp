#include "pch.hpp"
#include "Image/Image_logo.h"
#include "Image/Image_c4.h"
ID3D11ShaderResourceView* m_logo, * m_c4;

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
bool g_SwapChainOccluded = false;
UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
int 当前选中 = 1, 子菜单选中 = 1;
static constexpr const char* l_title[] = { "Gamers", "AimBot", "Radar", "Collect" };
const float 完全透明背景[4] = { 0, 0, 0, 0 };
const float 调试背景[4] = { 0.06f, 0.56f, 0.56f, 1.00f }; // 保持原始alpha值
using namespace Colors;
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
        const std::string fontPath = conf::SysFontDir + "msyh.ttc";
        const std::string boldFontPath = conf::SysFontDir + "msyhbd.ttc";
        const std::string quartzPath = conf::ExeFontDir + "Quartz.ttf";
        const std::string weaponiocPath = conf::ExeFontDir + "Weaponioc.ttf";
        const std::string stratum2Path = conf::ExeFontDir + "stratum2.ttf";
        const std::string icoPath = conf::ExeFontDir + "SHRIMP.ttf";
        sys::default_18 = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 18.0f * sys::dpi, &config, io.Fonts->GetGlyphRangesChineseFull());
        sys::default_22 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 22.0f * sys::dpi, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        sys::default_28 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 28.0f * sys::dpi);
        sys::default_48 = io.Fonts->AddFontFromFileTTF(boldFontPath.c_str(), 48.0f * sys::dpi);
        sys::Quartz = io.Fonts->AddFontFromFileTTF(quartzPath.c_str(), 32.0f * sys::dpi);
        sys::Weaponioc = io.Fonts->AddFontFromFileTTF(weaponiocPath.c_str(), 64.0f * sys::dpi);
        sys::stratum2_18 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 18.0f * sys::dpi);
        sys::stratum2_32 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 32.0f * sys::dpi);
        sys::stratum2_64 = io.Fonts->AddFontFromFileTTF(stratum2Path.c_str(), 64.0f * sys::dpi);
        sys::ico = io.Fonts->AddFontFromFileTTF(icoPath.c_str(), 48.0f * sys::dpi);
        io.FontDefault = sys::default_18;
        io.Fonts->Build();
    }    
    /* 初始化风格与颜色 */
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        style->ScaleAllSizes(sys::dpi);
        style->WindowPadding = ImVec2(0, 0);                   // 窗口填充
        style->WindowRounding = 5.0f * sys::dpi;               // 窗口圆角
        style->FramePadding = ImVec2(1 * sys::dpi, 1 * sys::dpi);// 控件内边距
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


        const ImVec4 dark_bg(0.133f, 0.114f, 0.173f, 1.0f);      // #221D2C
        const ImVec4 child_bg(0.161f, 0.141f, 0.208f, 1.0f);     // #292435
        const ImVec4 popup_bg(0.098f, 0.082f, 0.129f, 0.98f);    // #191521
        style->Colors[ImGuiCol_WindowBg] = dark_bg;
        style->Colors[ImGuiCol_ChildBg] = child_bg;
        style->Colors[ImGuiCol_PopupBg] = popup_bg;
        style->Colors[ImGuiCol_Text] = ImVec4(0.85f, 0.95f, 0.92f, 1.00f); // 冷白微青
        style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.60f, 1.00f);
        const ImVec4 emerald(0.0f, 0.71f, 0.67f, 1.0f);          // #00B5AD
        const ImVec4 emerald_trans(0.0f, 0.71f, 0.67f, 0.6f);
        style->Colors[ImGuiCol_Border] = ImVec4(emerald.x, emerald.y, emerald.z, 0.65f);
        style->Colors[ImGuiCol_CheckMark] = emerald;
        style->Colors[ImGuiCol_SliderGrab] = emerald_trans;
        style->Colors[ImGuiCol_SliderGrabActive] = emerald;
        style->Colors[ImGuiCol_Button] = ImVec4(emerald.x, emerald.y, emerald.z, 0.4f);
        style->Colors[ImGuiCol_ButtonHovered] = ImVec4(emerald.x, emerald.y, emerald.z, 0.8f);
        style->Colors[ImGuiCol_ButtonActive] = ImVec4(emerald.x, emerald.y, emerald.z, 1.0f);
        style->Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.07f, 0.10f, 1.00f);       // 深底
        style->Colors[ImGuiCol_TitleBgActive] = ImVec4(emerald.x * 0.3f, emerald.y * 0.3f, emerald.z * 0.3f, 1.0f);
        style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05f, 0.05f, 0.08f, 0.75f);
        // 特殊效果增强 ================================
        // 输入框聚焦光晕
        //style->Colors[ImGuiCol_FrameBgActive] = ImVec4(emerald.x, emerald.y, emerald.z, 0.3f);
        //style->FrameBorderSize = 1.0f;
        style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(emerald.x, emerald.y, emerald.z, 0.4f);
        style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(emerald.x, emerald.y, emerald.z, 0.7f);
        style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(emerald.x, emerald.y, emerald.z, 1.0f);
    
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
    LOG_INFO("ImGui已卸载!");
    /* 资源清理 */
    ReleaseImage();
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
void 绘制文本(ImDrawList* draw,ImVec2 pos,ImU32 col, const char* text)
{   
    
    std::string utf8_str = ToUTF8(text,936);
    draw->AddText(pos, col, utf8_str.c_str());
}
void 绘制文本_Fonts(ImDrawList* draw,ImFont* font, float font_size, ImVec2 pos, ImU32 col, const char* text)
{
    std::string utf8_str = ToUTF8(text, 936);
    draw->AddText(font, std::round(font_size * sys::dpi), pos, col, utf8_str.c_str());
}
/* 加载图片资源 */
void LoadImageFile()
{
    m_logo = LoadTextureImageFromMemory(aclogo, sizeof(aclogo));
    m_c4 = LoadTextureImageFromMemory(acc4, sizeof(acc4));
}
/* 手动释放资源 */
void ReleaseImage()
{
    ReleaseTextureResource(m_logo);
    ReleaseTextureResource(m_c4);
}
/* 绘制菜单 */
void _RenderFrame()
{
    /* 开始渲染 */
    ImVec2 size = { 350.0f,700.0f };
    ImGui::SetNextWindowSize(size);
    if (ImGui::Begin("imp.main", nullptr, 1 | 8 | 16)) {
        ImVec2 pos = ImGui::GetWindowPos();
        ImDrawList* draw = ImGui::GetWindowDrawList();
        bool 鼠标是否在加号中 = _IsMouseRect(ImVec2(pos.x + 320.0f, pos.y + 15.0f), ImVec2(20.0f,20.0f), ImGui::GetMousePos());
        draw->AddImageRounded(到图片指针(m_logo), pos, ImVec2(pos.x + 48.0f, pos.y + 48.0f), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), IM_COL32(255, 255, 255, 255), 50.0f, ImDrawFlags_RoundCornersAll);        
        draw->AddLine(ImVec2(pos.x + 325.0f, pos.y + 25.0f), ImVec2(pos.x + 335.0f, pos.y + 25.0f), toColor(浅灰), 2.0f);
        draw->AddLine(ImVec2(pos.x + 330.0f, pos.y + 20.0f), ImVec2(pos.x + 330.0f, pos.y + 30.0f), toColor(浅灰), 2.0f);
        draw->AddCircle(ImVec2(pos.x + 330.0f, pos.y + 25.0f), 10.0f, toColor(鼠标是否在加号中 ? 灰色 : 白色), ImDrawFlags_RoundCornersAll, 2.0f);
        draw->AddText(sys::stratum2_32, 32.0f * sys::dpi, ImVec2(pos.x + 140.0f, pos.y), toColor(青绿), "Shrimp");
        draw->AddText(sys::stratum2_18, 18.0f * sys::dpi, ImVec2(pos.x + 150.0f, pos.y + 30.0f), toColor(灰色), CS2_VERSION);
        ImGui::SetCursorPos(ImVec2(25.0f, 652.0f));
        if (创建菜单导航按钮(draw,当前选中 == 1 ? "6" : "5", 1,pos)) {
            当前选中 = 1;
        }
        if (创建菜单导航按钮(draw, 当前选中 == 2 ? "0" : "2", 2, pos))
        {
            当前选中 = 2;
        }
        if (创建菜单导航按钮(draw, 当前选中 == 3 ? "7" : "8", 3, pos))
        {
            当前选中 = 3;
        }
        if (创建菜单导航按钮(draw, 当前选中 == 4 ? "3" : "4", 4, pos))
        {
            当前选中 = 4;
        }
        /* 菜单导航 */
            {
            if (当前选中 == 1)
            {
                创建子菜单分类按钮(draw, "玩家绘制", 子菜单选中, pos, 展开状态::Player::Get().玩家绘制);
            }
            switch (子菜单选中) {
            case 1:
                _菜单分类_透视();       // 透视
                break;
            case 2:
                _菜单分类_自瞄();    // 自瞄
                break;
            case 3:
                _菜单分类_扳机();   // 扳机
                break;
            case 4:
                _菜单分类_雷达();     // 雷达
                break;
            case 5:
                _菜单分类_道具();      // 道具
                break;
            case 6:
                _菜单分类_点位();    // 点位
                break;
            case 7:
                _菜单分类_设置();  // 设置
                break;
            case 8:
                _菜单分类_外设();    // 外设
                break;
            default:
                // 处理未知菜单索引（如默认菜单或错误状态）
                break;
            }
            }
        ImGui::End();
    }
    /* 结束渲染 */
}

bool 创建菜单导航按钮(ImDrawList* draw,const char* ico, int selected_ID,ImVec2 pos)
{
    bool ret = false;
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button + 1, IM_COL32(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button + 2, IM_COL32(0, 0, 0, 0));
    ImGui::PushID("cddh" + (char)selected_ID);
    ret = ImGui::Button("##", ImVec2(52, 52));
    ImGui::PopID();
    ImGui::PopStyleColor(3);
    draw->AddText(sys::ico, 当前选中 == selected_ID ? 48.0f : 32.0f * sys::dpi, ImVec2(ImGui::GetItemRectMin().x, pos.y + 650.0f), toColor(当前选中 == selected_ID ? 青绿 : 灰色), ico);
    if (selected_ID != 当前选中) {
        draw->AddText(sys::stratum2_18, 16.0f * sys::dpi, ImVec2(ImGui::GetItemRectMin().x - 5.0f, pos.y + 680.0f), toColor(灰色), l_title[selected_ID - 1]);
    }
    ImGui::SameLine(NULL,30.0f);
    return ret;
}
void 创建子菜单分类按钮(ImDrawList* draw, const char* title, int selectde_ID, ImVec2 pos, bool 状态)
{
    draw->AddRectFilled(ImVec2(pos.x + 10.0f, pos.y + ImGui::GetCursorScreenPos().y), ImVec2(pos.x + 380.0f, pos.y + ImGui::GetCursorScreenPos().y + 48.0f), (0.133f, 0.114f, 0.173f, 0.3f), 5.0f);
}
void _菜单分类_透视()
{ 

}
void _菜单分类_自瞄()
{ }
void _菜单分类_扳机()
{ }
void _菜单分类_雷达()
{ }
void _菜单分类_道具()
{ }
void _菜单分类_点位()
{ }
void _菜单分类_设置()
{ }
void _菜单分类_外设()
{ }

ImVec2 _addImVec2(ImVec2 p1, ImVec2 p2)
{
    return  ImVec2(p1.x + p2.x, p1.y + p2.y);
}
ImVec3 _addImVec3(ImVec3 p1, ImVec3 p2)
{
    return  p1 + p2;
}
bool _IsMouseRect(ImVec2 pos, ImVec2 size,ImVec2 MousePos)
{
    return MousePos.x > pos.x && MousePos.x <= pos.x + size.x && MousePos.y > pos.y && MousePos.y <= pos.y + size.y;
}
