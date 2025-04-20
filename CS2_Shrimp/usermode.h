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
#include <dwmapi.h>
#include <string>

/* ext/includes */
#include "ext/nlohmann/json.hpp"
#include "ext/easywsclient/easywsclient.hpp"
#include "ext/fnv1a/fnv1a.hpp"
#include "ext/curl/include/curl.h"

#include "EC/EC.h"
#include "EC/common.h"
#include "EC/MemoryX64.h"