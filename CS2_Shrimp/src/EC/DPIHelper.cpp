#include "pch.hpp"
#include <cmath>

float DPIScaling::GetSystemScale() {
    if (auto scale = GetModernDPI()) {
        return scale;
    }
    return GetCompatibleDPI();
}

float DPIScaling::GetModernDPI() {
    static HMODULE hUser32 = LoadLibraryW(L"User32.dll");
    if (!hUser32) return 0.0f;

    using GetDpiForWindowProc = UINT(WINAPI*)(HWND);
    auto pGetDpiForWindow = reinterpret_cast<GetDpiForWindowProc>(
        GetProcAddress(hUser32, "GetDpiForWindow"));

    if (pGetDpiForWindow && GetDesktopWindow()) {
        return pGetDpiForWindow(GetDesktopWindow()) / 96.0f;
    }
    return 0.0f;
}

float DPIScaling::GetCompatibleDPI() {
    HDC hdc = GetDC(nullptr);
    if (!hdc) return 1.0f;

    const int horzSize = GetDeviceCaps(hdc, HORZSIZE);
    const int vertSize = GetDeviceCaps(hdc, VERTSIZE);
    const int horzRes = GetDeviceCaps(hdc, HORZRES);
    const int vertRes = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(nullptr, hdc);

    const float dpiX = (horzRes * 25.4f) / horzSize;
    const float dpiY = (vertRes * 25.4f) / vertSize;

    if (std::abs(dpiX - dpiY) > 10.0f || dpiX < 24.0f || dpiX > 600.0f) {
        return 1.0f;
    }
    return std::round(dpiX / 96.0f * 4) / 4.0f;
}