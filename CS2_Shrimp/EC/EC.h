#pragma once
#include "imgui.h"
namespace sys
{
	inline BOOL debug = false;
	inline BOOL norefresh = false;//��ֹDMA�ڲ�ˢ��
	inline BOOL ReadType = false;//��:WINAPI		��:DMA
	inline BOOL loop = false;//��Ҫ���߳�ѭ������
	inline uint64_t ʣ��ʱ�� = NULL;
	inline HWND hWnd = NULL;//���ھ��
	inline DWORD dwPID = NULL;
	inline FLOAT dpi = NULL;
	inline uint32_t ScreenX = NULL;
	inline uint32_t ScreenY = NULL;
	inline uint32_t SizeX = NULL;
	inline uint32_t SizeY = NULL;
	inline BOOL ������״̬ = NULL;
	inline BOOL ��ֱͬ�� = NULL;
	inline ImFont* _Ĭ������_18 = nullptr;
	inline ImFont* _Ĭ������_22 = nullptr;
	inline ImFont* _Ĭ������_28 = nullptr;
	inline ImFont* _Ĭ������_48 = nullptr;
	inline ImFont* _��������2_18 = nullptr;
	inline ImFont* _��������2_32 = nullptr;
	inline ImFont* _��������2_64 = nullptr;
	inline ImFont* _��������_32 = nullptr;
	inline ImFont* _���ͼ��_28 = nullptr;
	inline ImFont* _�������� = nullptr;
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
void �����ı�(ImVec2 pos, ImU32 col, std::wstring chinese_text);
BOOL UpdateWindowData(HWND hWnd, HWND im_hWnd);
void LoadFonts();
BOOL startInit();
HWND FindCS2Window() noexcept;
