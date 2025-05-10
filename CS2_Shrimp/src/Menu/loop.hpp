#pragma once
extern ID3D11ShaderResourceView* m_logo, * m_c4;
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern bool g_SwapChainOccluded;
extern UINT g_ResizeWidth;
extern UINT g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
using namespace std;
namespace fs = std::filesystem;
namespace sys
{
    inline BOOL debug = false;
    inline BOOL norefresh = false;//禁止DMA内部刷新
    inline BOOL ReadType = false;//假:WINAPI		真:DMA
    inline BOOL loop = false;//主要的线程循环条件
    inline uint64_t 剩余时间 = NULL;
    inline HWND hWnd = NULL;//窗口句柄
    inline DWORD dwPID = NULL;
    inline float dpi = NULL;
    inline uint32_t ScreenX = NULL;
    inline uint32_t ScreenY = NULL;
    inline uint32_t SizeX = NULL;
    inline uint32_t SizeY = NULL;
    inline BOOL 主界面状态 = NULL;
    inline BOOL 垂直同步 = NULL;
    inline ImFont* default_18 = nullptr;
    inline ImFont* default_22 = nullptr;
    inline ImFont* default_28 = nullptr;
    inline ImFont* default_48 = nullptr;
    inline ImFont* stratum2_18 = nullptr;
    inline ImFont* stratum2_32 = nullptr;
    inline ImFont* stratum2_64 = nullptr;
    inline ImFont* Quartz = nullptr;
    inline ImFont* Weaponioc = nullptr;
    inline ImFont* ico = nullptr;
}

namespace imp
{
    inline uint64_t engineBase = 0;
    inline uint64_t clientBase = 0;
	inline nlohmann::json Weapon = {};
	inline nlohmann::json Config = {};
}

void menuloop();
bool 创建菜单导航按钮(ImDrawList* draw, const char* ico, int selected_ID, ImVec2 pos);
void 创建子菜单分类按钮(ImDrawList* draw, const char* title, int selectde_ID, ImVec2 pos, bool 状态);
void _菜单分类_透视();
void _菜单分类_自瞄();
void _菜单分类_扳机();
void _菜单分类_雷达();
void _菜单分类_道具();
void _菜单分类_点位();
void _菜单分类_设置();
void _菜单分类_外设();
ImVec2 _addImVec2(ImVec2 p1, ImVec2 p2);
ImVec3 _addImVec3(ImVec3 p1, ImVec3 p2);
bool _IsMouseRect(ImVec2 pos, ImVec2 size, ImVec2 MousePos);
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL UpdateWindowData(HWND hWnd, HWND im_hWnd);
void LoadImageFile();
ImVec4 HexToImVec4(uint32_t color);
void 绘制文本(ImDrawList* draw, ImVec2 pos, ImU32 col, const char* text);
void 绘制文本_Fonts(ImDrawList* draw, ImFont* font, float font_size, ImVec2 pos, ImU32 col, const char* text);
void _RenderFrame();
void ReleaseImage();