#include "pch.hpp"

using namespace std;
int main(int argc, char* argv[])
{
    system("title 【Shrimp】Exterior auto - 1.413");
    constexpr array<string_view, 4> validArgs = { "debug", "norefresh", "WINAPI", "DMA" };
    bool hasValidArg = false;
    for (int i = 1; i < argc; ++i)
    {
        string_view arg(argv[i]);
        if (arg == "debug") {
            sys::debug = true;
            hasValidArg = true;
        }
        else if (arg == "norefresh") {
            sys::norefresh = true;
            hasValidArg = true;
        }
        else if (arg == "WINAPI") {
            sys::ReadType = false;
            hasValidArg = true;
        }
        else if (arg == "DMA") {
            sys::ReadType = true;
            hasValidArg = true;
        }
        else {
            LOG_WARNING("未知参数: {}", arg);
        }
    }
    if (!sys::debug && !hasValidArg)
    {
        LOG_WARNING("缺少有效参数！可用参数：");
        for (const auto& arg : validArgs) {
            cout << "  " << arg << '\n';
        }
        cout << "按回车键退出...";
        cin.get();
        return EXIT_FAILURE;
    }
    if (sys::debug) {
        LOG_WARNING("调试模式已激活");
    }
    sys::剩余时间 = 869918530;
    sys::loop = sys::剩余时间 > 0;
    if (sys::debug)
    {
        sys::hWnd = FindWindowW(L"Progman", L"Program Manager");
        return true;
    }
    LOG_INFO("获取游戏进程PID...");
    if (sys::ReadType)
    {
        // DMA初始化逻辑保留
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
        config_data_t config_data = {};     

        imp::imgui();
    }
    return EXIT_SUCCESS;
}