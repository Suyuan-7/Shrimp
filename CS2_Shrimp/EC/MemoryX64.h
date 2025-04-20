#pragma once
#include "../usermode.h"

std::optional<uintptr_t> GetModuleBaseByPidEx(DWORD pid, const wchar_t* moduleName);
