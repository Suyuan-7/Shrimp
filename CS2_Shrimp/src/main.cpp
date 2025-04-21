#include "pch.hpp"

using namespace std;
int main(int argc, char* argv[])
{
    system("title ��Shrimp��Exterior auto - 1.413");
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
    if (!sys::debug && !hasValidArg)
    {
        LOG_WARNING("ȱ����Ч���������ò�����");
        for (const auto& arg : validArgs) {
            cout << "  " << arg << '\n';
        }
        cout << "���س����˳�...";
        cin.get();
        return EXIT_FAILURE;
    }
    if (sys::debug) {
        LOG_WARNING("����ģʽ�Ѽ���");
    }
    sys::ʣ��ʱ�� = 869918530;
    sys::loop = sys::ʣ��ʱ�� > 0;
    if (sys::debug)
    {
        sys::hWnd = FindWindowW(L"Progman", L"Program Manager");
        return true;
    }
    LOG_INFO("��ȡ��Ϸ����PID...");
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
        config_data_t config_data = {};     

        imp::imgui();
    }
    return EXIT_SUCCESS;
}