#include "pch.hpp"


extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern bool g_SwapChainOccluded;
extern UINT g_ResizeWidth, g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
// 将宽字符字符串（UTF-16）转换为 UTF-8
std::string ToUTF8(const wchar_t* wideStr) {
    if (!wideStr || !*wideStr) return {};

    // 计算 UTF-8 字符串长度
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0) return {};

    // 分配内存并转换
    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str.data(), utf8Len, nullptr, nullptr);

    // 去除末尾的 '\0'
    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}
// 将宽字符字符串（UTF-16）转换为 UTF-8
std::string ToUTF8(const std::wstring& wideStr) {
    return ToUTF8(wideStr.c_str());
}
// 将多字节字符串（如 GBK、Shift-JIS）转换为 UTF-8
std::string ToUTF8(const char* text, UINT codePage /*= CP_ACP*/) {
    if (!text || !*text) return {};

    // 如果已经是 UTF-8，直接返回
    if (codePage == CP_UTF8) {
        return text;
    }

    // Step 1: 将多字节字符串转换为宽字符（UTF-16）
    int wideLen = MultiByteToWideChar(codePage, 0, text, -1, nullptr, 0);
    if (wideLen == 0) return {};

    std::wstring wideStr(wideLen, L'\0');
    MultiByteToWideChar(codePage, 0, text, -1, wideStr.data(), wideLen);

    // Step 2: 将宽字符（UTF-16）转换为 UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0) return {};

    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, utf8Str.data(), utf8Len, nullptr, nullptr);

    // 去除末尾的 '\0'
    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}
// 重载版本，支持 std::string 输入
std::string ToUTF8(const std::string& text, UINT codePage /*= CP_ACP*/) {
    return ToUTF8(text.c_str(), codePage);
}

//std::string ToUTF8(const std::wstring& wideStr) {
//    if (wideStr.empty()) return {};
//
//    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideStr.data(), wideStr.size(), nullptr, 0, nullptr, nullptr);
//    std::string result(sizeNeeded, '\0');
//    WideCharToMultiByte(CP_UTF8, 0, wideStr.data(), wideStr.size(), result.data(), sizeNeeded, nullptr, nullptr);
//    return result;
//}
HWND FindCS2Window() noexcept {
    constexpr std::wstring_view classNames[] = { L"SDL_app", L"SDL_app" };
    constexpr std::wstring_view windowTitles[] = { L"Counter-Strike 2", L"反恐精英：全球攻势" };

    for (auto className : classNames) {
        for (auto title : windowTitles) {
            if (HWND hWnd = FindWindowW(className.data(), title.data())) {
                return hWnd;
            }
        }
    }
    return nullptr;
}
/* 获取系统字体 */
std::string GetSystemFontDirectory() {
    char path[MAX_PATH];

    // 使用Windows API获取字体目录
    if (SUCCEEDED(SHGetFolderPathA(
        NULL,                       // hwndOwner
        CSIDL_FONTS,                // 系统字体目录标识
        NULL,                       // hToken
        SHGFP_TYPE_CURRENT,         // 获取当前路径
        path)))                     // 输出缓冲区
    {
        return std::string(path);
    }
    return ""; // 失败返回空字符串
}
/* 获取运行目录 */
std::string GetExecutableDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH); // Windows API
    return fs::path(buffer).parent_path().string();
}
namespace conf
{
    /* 本地字体目录 */
    std::string ExeFontDir = GetExecutableDirectory() + "\\fonts\\";
    /* 系统字体 */
    std::string SysFontDir = GetSystemFontDirectory() + "\\";
    /* 根目录 */
    std::string Dir_Root = GetExecutableDirectory();
    /* 配置项目录 */
    std::string Dir_config = GetExecutableDirectory() + "\\config\\";
}
namespace Colors {
    const int 黑色 = 0x000000;
    const int 白色 = 0xFFFFFF;
    const int 红色 = 0xFF0000;
    const int 绿色 = 0x00FF00;
    const int 蓝色 = 0x0000FF;
    const int 黄色 = 0xFFFF00;
    const int 青色 = 0x00FFFF;
    const int 品红 = 0xFF00FF;
    const int 紫色 = 0x800080;
    const int 橙色 = 0xFFA500;
    const int 灰色 = 0x808080;
    const int 浅灰 = 0xD3D3D3;
    const int 深灰 = 0x4F4F4F;
    const int 天蓝 = 0x87CEEB;
    const int 粉红 = 0xFFC0CB;
    const int 金色 = 0xFFD700;
    const int 银白 = 0xECECEC;
    const int 茶色 = 0x8B4513;
    const int 青绿 = 0x00FA9A;
    const int 透明色 = 0x00000000;
}

