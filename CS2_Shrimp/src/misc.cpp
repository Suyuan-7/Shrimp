#include "pch.hpp"


extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern bool g_SwapChainOccluded;
extern UINT g_ResizeWidth, g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
// �����ַ��ַ�����UTF-16��ת��Ϊ UTF-8
std::string ToUTF8(const wchar_t* wideStr) {
    if (!wideStr || !*wideStr) return {};

    // ���� UTF-8 �ַ�������
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0) return {};

    // �����ڴ沢ת��
    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str.data(), utf8Len, nullptr, nullptr);

    // ȥ��ĩβ�� '\0'
    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}
// �����ַ��ַ�����UTF-16��ת��Ϊ UTF-8
std::string ToUTF8(const std::wstring& wideStr) {
    return ToUTF8(wideStr.c_str());
}
// �����ֽ��ַ������� GBK��Shift-JIS��ת��Ϊ UTF-8
std::string ToUTF8(const char* text, UINT codePage /*= CP_ACP*/) {
    if (!text || !*text) return {};

    // ����Ѿ��� UTF-8��ֱ�ӷ���
    if (codePage == CP_UTF8) {
        return text;
    }

    // Step 1: �����ֽ��ַ���ת��Ϊ���ַ���UTF-16��
    int wideLen = MultiByteToWideChar(codePage, 0, text, -1, nullptr, 0);
    if (wideLen == 0) return {};

    std::wstring wideStr(wideLen, L'\0');
    MultiByteToWideChar(codePage, 0, text, -1, wideStr.data(), wideLen);

    // Step 2: �����ַ���UTF-16��ת��Ϊ UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Len == 0) return {};

    std::string utf8Str(utf8Len, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, utf8Str.data(), utf8Len, nullptr, nullptr);

    // ȥ��ĩβ�� '\0'
    if (!utf8Str.empty() && utf8Str.back() == '\0') {
        utf8Str.pop_back();
    }

    return utf8Str;
}
// ���ذ汾��֧�� std::string ����
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
    constexpr std::wstring_view windowTitles[] = { L"Counter-Strike 2", L"���־�Ӣ��ȫ����" };

    for (auto className : classNames) {
        for (auto title : windowTitles) {
            if (HWND hWnd = FindWindowW(className.data(), title.data())) {
                return hWnd;
            }
        }
    }
    return nullptr;
}
/* ��ȡϵͳ���� */
std::string GetSystemFontDirectory() {
    char path[MAX_PATH];

    // ʹ��Windows API��ȡ����Ŀ¼
    if (SUCCEEDED(SHGetFolderPathA(
        NULL,                       // hwndOwner
        CSIDL_FONTS,                // ϵͳ����Ŀ¼��ʶ
        NULL,                       // hToken
        SHGFP_TYPE_CURRENT,         // ��ȡ��ǰ·��
        path)))                     // ���������
    {
        return std::string(path);
    }
    return ""; // ʧ�ܷ��ؿ��ַ���
}
/* ��ȡ����Ŀ¼ */
std::string GetExecutableDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH); // Windows API
    return fs::path(buffer).parent_path().string();
}
namespace conf
{
    /* ��������Ŀ¼ */
    std::string ExeFontDir = GetExecutableDirectory() + "\\fonts\\";
    /* ϵͳ���� */
    std::string SysFontDir = GetSystemFontDirectory() + "\\";
    /* ��Ŀ¼ */
    std::string Dir_Root = GetExecutableDirectory();
    /* ������Ŀ¼ */
    std::string Dir_config = GetExecutableDirectory() + "\\config\\";
}
namespace Colors {
    const int ��ɫ = 0x000000;
    const int ��ɫ = 0xFFFFFF;
    const int ��ɫ = 0xFF0000;
    const int ��ɫ = 0x00FF00;
    const int ��ɫ = 0x0000FF;
    const int ��ɫ = 0xFFFF00;
    const int ��ɫ = 0x00FFFF;
    const int Ʒ�� = 0xFF00FF;
    const int ��ɫ = 0x800080;
    const int ��ɫ = 0xFFA500;
    const int ��ɫ = 0x808080;
    const int ǳ�� = 0xD3D3D3;
    const int ��� = 0x4F4F4F;
    const int ���� = 0x87CEEB;
    const int �ۺ� = 0xFFC0CB;
    const int ��ɫ = 0xFFD700;
    const int ���� = 0xECECEC;
    const int ��ɫ = 0x8B4513;
    const int ���� = 0x00FA9A;
    const int ͸��ɫ = 0x00000000;
}

