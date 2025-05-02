#pragma once
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
    inline BOOL norefresh = false;//��ֹDMA�ڲ�ˢ��
    inline BOOL ReadType = false;//��:WINAPI		��:DMA
    inline BOOL loop = false;//��Ҫ���߳�ѭ������
    inline uint64_t ʣ��ʱ�� = NULL;
    inline HWND hWnd = NULL;//���ھ��
    inline DWORD dwPID = NULL;
    inline float dpi = NULL;
    inline uint32_t ScreenX = NULL;
    inline uint32_t ScreenY = NULL;
    inline uint32_t SizeX = NULL;
    inline uint32_t SizeY = NULL;
    inline BOOL ������״̬ = NULL;
    inline BOOL ��ֱͬ�� = NULL;
    inline ImFont* default_18 = nullptr;
    inline ImFont* default_22 = nullptr;
    inline ImFont* default_28 = nullptr;
    inline ImFont* default_48 = nullptr;
    inline ImFont* stratum2_18 = nullptr;
    inline ImFont* stratum2_32 = nullptr;
    inline ImFont* stratum2_64 = nullptr;
    inline ImFont* Quartz = nullptr;
    inline ImFont* Weaponioc = nullptr;
}

namespace imp
{
    inline uint64_t engineBase = 0;
    inline uint64_t clientBase = 0;
	inline nlohmann::json Weapon = {};
	inline nlohmann::json Config = {};
}

void menuloop();