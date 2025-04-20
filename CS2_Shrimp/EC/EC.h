#pragma once
#include "imgui.h"
namespace sys
{
	inline BOOL debug = false;
	inline BOOL norefresh = false;//禁止DMA内部刷新
	inline BOOL ReadType = false;//假:WINAPI		真:DMA
	inline BOOL loop = false;//主要的线程循环条件
	inline uint64_t 剩余时间 = NULL;
	inline HWND hWnd = NULL;//窗口句柄
	inline DWORD dwPID = NULL;
	inline FLOAT dpi = NULL;
	inline uint32_t ScreenX = NULL;
	inline uint32_t ScreenY = NULL;
	inline uint32_t SizeX = NULL;
	inline uint32_t SizeY = NULL;
	inline BOOL 主界面状态 = NULL;
	inline BOOL 垂直同步 = NULL;
	inline ImFont* _默认字体_18 = nullptr;
	inline ImFont* _默认字体_22 = nullptr;
	inline ImFont* _默认字体_28 = nullptr;
	inline ImFont* _默认字体_48 = nullptr;
	inline ImFont* _数字字体2_18 = nullptr;
	inline ImFont* _数字字体2_32 = nullptr;
	inline ImFont* _数字字体2_64 = nullptr;
	inline ImFont* _数字字体_32 = nullptr;
	inline ImFont* _软件图标_28 = nullptr;
	inline ImFont* _武器字体 = nullptr;
}

namespace imp
{
	void imgui();
	inline uint64_t engineBase = 0;
	inline uint64_t clientBase = 0;
	inline nlohmann::json Weapon = {};
	inline nlohmann::json Config = {};
}

float GetSystemDPI();
std::string WideToUTF8(const std::wstring& wstr);
void InputHandler(HWND hWnd);
void 绘制文本(ImVec2 pos, ImU32 col, std::wstring chinese_text);
BOOL UpdateWindowData(HWND hWnd, HWND im_hWnd);
void LoadFonts();
BOOL startInit();
HWND FindCS2Window() noexcept;
