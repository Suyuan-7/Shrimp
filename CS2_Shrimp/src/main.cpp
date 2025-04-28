#include "pch.hpp"
int main(int argc, char* argv[])
{
    demomain();
    sys::dpi = 1.0f;
    system("title 【Shrimp】Exterior auto - " CS2_VERSION);
    constexpr array<string_view, 5> validArgs = { "debug", "norefresh", "WINAPI", "DMA","dpi=1.0" };
    for (int i = 1; i < argc; ++i)
    {
        string_view arg(argv[i]);
        string s = argv[i];
        if (arg == "debug") {
            sys::debug = true;
        }
        else if (arg == "norefresh") {
            sys::norefresh = true;
        }
        else if (arg == "WINAPI") {
            sys::ReadType = false;
        }
        else if (arg == "DMA") {
            sys::ReadType = true;
        }
        else if (s.size ()>=4 && s.substr(0,4)=="dpi=") {
            string dpi;
            dpi = s.substr(4);
            char* endPtr;
            float DPI = strtod(dpi.c_str(), &endPtr);
            if (endPtr == dpi.c_str() + dpi.size()) {
                sys::dpi = DPI;
                LOG_INFO("适配DPI:%.2f", DPI);
            }
            else {
                sys::dpi = 1.0f;
                LOG_ERROR("适配DPI失败,回退到默认值100%!");
            }
        }
        else {
            sys::dpi = 1.0f;
            LOG_ERROR("适配DPI失败,回退到默认值100%!");
        }
    }
    if (sys::debug) {
        LOG_WARNING("调试模式");
    }
    sys::剩余时间 = 869918530;
    sys::loop = sys::剩余时间 > 0;
    LOG_INFO("获取游戏进程PID...");
    if (sys::debug)
    {
        sys::hWnd = FindWindowW(L"Progman", L"Program Manager");
    }
    else 
    {
        if (sys::ReadType)
        {
            /* DMA初始化逻辑保留 */
        }
        else {
            const wchar_t* windowTitles[] = {
                L"Counter-Strike 2",
                L"反恐精英：全球攻势"
            };
            HWND hWnd = nullptr;
            for (const auto title : windowTitles)
            {
                hWnd = FindWindowW(L"SDL_app", title);
                if (hWnd) break;
            }
            if (!hWnd)
            {
                LOG_WARNING("游戏未运行!");
                return EXIT_FAILURE;
            }
            sys::hWnd = hWnd;
            GetWindowThreadProcessId(sys::hWnd, &sys::dwPID);
            if (!sys::dwPID)
            {
                LOG_WARNING("游戏未运行!");
                return EXIT_FAILURE;
            }
            LOG_INFO("游戏句柄%d进程PID%d客户端模块地址%dengine2%d", sys::hWnd, sys::dwPID,imp::clientBase,imp::engineBase);
        }
    }
    config_data_t config_data = {};
    //menuloop();
    return EXIT_SUCCESS;
}