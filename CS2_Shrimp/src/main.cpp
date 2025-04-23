#include "pch.hpp"

using namespace std;
int main(int argc, char* argv[])
{
    system("title ��Shrimp��Exterior auto - " CS2_VERSION);
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
            LOG_WARNING("δ֪����: {}", arg);
        }
    }
    if (sys::debug) {
        LOG_WARNING("����ģʽ�Ѽ���");
    }
    sys::ʣ��ʱ�� = 869918530;
    sys::loop = sys::ʣ��ʱ�� > 0;
    LOG_INFO("��ȡ��Ϸ����PID...");
    if (sys::debug)
    {
        sys::hWnd = FindWindowW(L"Progman", L"Program Manager");
    }
    else 
    {
        if (sys::ReadType)
        {
            // DMA��ʼ���߼�����
        }
        else {
            const wchar_t* windowTitles[] = {
                L"Counter-Strike 2",
                L"���־�Ӣ��ȫ����"
            };
            HWND hWnd = nullptr;
            for (const auto title : windowTitles)
            {
                hWnd = FindWindowW(L"SDL_app", title);
                if (hWnd) break;
            }
            if (!hWnd)
            {
                LOG_WARNING("��Ϸδ����!");
                return EXIT_FAILURE;
            }
            sys::hWnd = hWnd;
            GetWindowThreadProcessId(sys::hWnd, &sys::dwPID);
            if (!sys::dwPID)
            {
                LOG_WARNING("��Ϸδ����!");
                return EXIT_FAILURE;
            }
            LOG_INFO("��Ϸ���%d����PID%d�ͻ���ģ���ַ%dengine2%d", sys::hWnd, sys::dwPID,imp::clientBase,imp::engineBase);
        }
    }
    config_data_t config_data = {};
    imp::menuloop();
    return EXIT_SUCCESS;
}