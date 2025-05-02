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
#include <shlobj.h>
#include <d3d11.h>
/* root */
#include "src/common.hpp"
/* ext/includes */
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"
#include "ext/curl/include/curl.h"
/* imgui */
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_add_widgets.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
///* src */
#include "src/misc.hpp"
#include "src/Menu/loop.hpp"
#include "src/utils/config.hpp"
#include "src/utils/utils.hpp"
#include "src/utils/exception.hpp"
#include "src/utils/dx_utils.hpp"