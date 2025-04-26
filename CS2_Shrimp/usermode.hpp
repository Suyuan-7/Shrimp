#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <fstream>
#include <set>
#include <regex>
#include <thread>
#include <stdio.h>
#include <string>
#include <winternl.h>
#include <psapi.h>
#include <format>
#include <optional>
#include <dwmapi.h>
#include <shlobj.h> /* ”√”⁄SHGetFolderPath */
/* ext/includes */
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"
#include "ext/curl/include/curl.h"
/* imgui */
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "imgui_add_widgets.h"
/* root */
#include "src/EC/common.hpp"
/* src */
#include "src/EC/EC.hpp"
#include "src/utils/config.hpp"
#include "src/utils/utils.hpp"
#include "src/utils/exception.hpp"
#include "src/utils/address.hpp"
#include "src/utils/memory.hpp"