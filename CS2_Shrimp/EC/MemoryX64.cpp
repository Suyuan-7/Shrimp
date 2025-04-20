#include "../usermode.h"
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
std::optional<uintptr_t> GetModuleBaseByPidEx(DWORD pid, const wchar_t* moduleName) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProcess) return std::nullopt;

    HMODULE hMods[1024];
    DWORD cbNeeded;
    std::optional<uintptr_t> result;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        const DWORD moduleCount = cbNeeded / sizeof(HMODULE);
        wchar_t modName[MAX_PATH];

        for (DWORD i = 0; i < moduleCount; i++) {
            if (GetModuleFileNameExW(hProcess, hMods[i], modName, MAX_PATH)) {
                if (wcsstr(modName, moduleName) != nullptr) { // Ä£ºýÆ¥ÅäÂ·¾¶
                    result = reinterpret_cast<uintptr_t>(hMods[i]);
                    break;
                }
            }
        }
    }
    CloseHandle(hProcess);
    return result;
}