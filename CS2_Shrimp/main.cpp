#include "usermode.h"
#include "EC/MemoryX64.h"
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
    system("title ��Shrimp��Exterior auto - 1.413");
    constexpr std::array<std::string_view, 4> validArgs = {
        "debug", "norefresh", "WINAPI", "DMA"
    };
    bool hasValidArg = false;
    for (int i = 1; i < argc; ++i) {
        std::string_view arg(argv[i]);
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
    if (!sys::debug && !hasValidArg) {
        LOG_WARNING("ȱ����Ч���������ò�����");
        for (auto& arg : validArgs) {
            std::cout << "  " << arg << '\n';
        }
        std::cout << "���س����˳�...";
        std::cin.get();
        return EXIT_FAILURE;
    }
    if (sys::debug) {
        LOG_WARNING("����ģʽ�Ѽ���");
    }
    sys::ʣ��ʱ�� = 869918530; // ʾ��ֵ
    auto startTime = std::chrono::steady_clock::now();
    if (startInit()) {
        imp::imgui(); // ������ѭ��
        cin.get();
    }
    else {
        throw std::runtime_error("��ʼ��ʧ��");
    }
    return EXIT_SUCCESS;
}

BOOL startInit()//��ʼ��.��ȡ��Ϸ���ھ��|����PID or DMA��س�ʼ��
{
    sys::loop = sys::ʣ��ʱ�� > 0;
    if (sys::debug)
    {
        sys::hWnd = FindWindowW(L"Progman", L"Program Manager");
        return 1;
    }

    LOG_INFO("��ȡ��Ϸ����PID...");
    if (sys::ReadType)
    {

    }
    else {
        HWND hWnd = FindWindowW(L"SDL_app", L"Counter-Strike 2");
        if (!hWnd) {
            hWnd = FindWindowW(L"SDL_app", L"���־�Ӣ��ȫ����");
        }
        if (!hWnd) {
            LOG_WARNING("��Ϸδ����!");
            return 0;
        }
        sys::hWnd = hWnd;
        GetWindowThreadProcessId(sys::hWnd, &sys::dwPID);
        if (auto base1 = GetModuleBaseByPidEx(sys::dwPID, L"client.dll")) {
            std::cout << "client.dll: 0x" << std::hex << *base1 << std::endl;
            imp::clientBase = *base1;
        }
        if (auto base2 = GetModuleBaseByPidEx(sys::dwPID, L"engine2.dll")) {
            std::cout << "engine2.dll: 0x" << std::hex << *base2 << std::endl;
            imp::engineBase = *base2;
        }
    }
    return 0;
}