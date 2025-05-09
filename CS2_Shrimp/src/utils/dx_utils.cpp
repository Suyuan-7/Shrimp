#include "pch.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <algorithm>
/* 从文件加载获取纹理ID */
//ID3D11ShaderResourceView* LoadTextureFromFile(const char* filename)
//{
//    const std::string imageDir = conf::Dir_image + filename;
//    int width, height, channels;
//    unsigned char* data = stbi_load(imageDir.c_str(),& width, &height, &channels, 4);
//            if (!data) return nullptr;
//
//            D3D11_TEXTURE2D_DESC desc = {};
//            desc.Width = width;
//            desc.Height = height;
//            desc.MipLevels = 1;
//            desc.ArraySize = 1;
//            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//            desc.SampleDesc.Count = 1;
//            desc.Usage = D3D11_USAGE_DEFAULT;
//            desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//            desc.CPUAccessFlags = 0;
//
//            D3D11_SUBRESOURCE_DATA initData = {};
//            initData.pSysMem = data;
//            initData.SysMemPitch = width * 4;
//            initData.SysMemSlicePitch = width * height * 4;
//
//            ID3D11Texture2D* texture = nullptr;
//            HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture);
//            stbi_image_free(data);
//            if (FAILED(hr)) return nullptr;
//
//            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//            srvDesc.Texture2D.MipLevels = 1;
//
//            ID3D11ShaderResourceView* srv = nullptr;
//            hr = g_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &srv);
//            texture->Release();
//
//            if (FAILED(hr)) return nullptr;
//
//            return srv;
//        }
/* 从内存加载纹理 */
ID3D11ShaderResourceView* LoadTextureImageFromMemory(const BYTE* data, size_t dataSize)
{
    if (data == nullptr || dataSize == 0 || dataSize > INT_MAX) {
        LOG_ERROR("Invalid input data or size exceeds INT_MAX");
        return nullptr;
    }
    int width, height, channels;
    unsigned char* imageData = stbi_load_from_memory(data, static_cast<int>(dataSize), &width, &height, &channels, 4);
    if (!imageData) {
        LOG_ERROR("STB_IMAGE failed to load: %s", stbi_failure_reason());
        return nullptr; // 图像加载失败
    }
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData;
    initData.SysMemPitch = width * 4; // 每行字节数（RGBA）
    initData.SysMemSlicePitch = width * height * 4; // 整张图像字节数
    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &initData, &texture);
    stbi_image_free(imageData); // 释放 stb_image 分配的内存

    if (FAILED(hr) || !texture) {
        LOG_ERROR("CreateTexture2D failed: HRESULT=0x%08X", hr);
        return nullptr; // 创建纹理失败
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    ID3D11ShaderResourceView* srv = nullptr;
    hr = g_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &srv);
    texture->Release(); // 不再需要纹理对象
    if (FAILED(hr) || !srv) {
        LOG_ERROR("CreateShaderResourceView failed: HRESULT=0x%08X", hr);
        return nullptr; // 创建 SRV 失败
    }
    return srv;
}
//* @brief 安全释放通过 LoadTextureImageFromMemory 加载的纹理资源
//* @param srv 要释放的着色器资源视图指针（允许传入 nullptr）
void ReleaseTextureResource(ID3D11ShaderResourceView * srv) {
    if (srv != nullptr) {
        srv->Release();
        srv = nullptr; // 可选：置空避免野指针（需通过引用或双重指针实现）
    }
}
bool InitDirectX(HWND hwnd)
        {
            DXGI_SWAP_CHAIN_DESC sd = {};
            sd.BufferCount = 2;
            sd.BufferDesc.Width = 0;
            sd.BufferDesc.Height = 0;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = hwnd;
            sd.SampleDesc.Count = 1;
            sd.Windowed = TRUE;
            sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            sd.Flags = 0;

            D3D_FEATURE_LEVEL featureLevel;
            HRESULT hr = D3D11CreateDeviceAndSwapChain(
                nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                nullptr, 0, D3D11_SDK_VERSION, &sd,
                &g_pSwapChain,
                &g_pd3dDevice,
                &featureLevel,
                &g_pd3dDeviceContext
            );
            if (FAILED(hr)) return false;

            // 创建渲染目标视图
            ID3D11Texture2D* pBackBuffer = nullptr;
            hr = g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            if (FAILED(hr)) return false;

            hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
            pBackBuffer->Release();

            if (FAILED(hr)) return false;

            return true;
        }
void Cleanup()
        {
            if (g_mainRenderTargetView) g_mainRenderTargetView->Release();
            if (g_pSwapChain) g_pSwapChain->Release();
            if (g_pd3dDeviceContext) g_pd3dDeviceContext->Release();
            if (g_pd3dDevice) g_pd3dDevice->Release();
        }
ImTextureID 到图片指针(ID3D11ShaderResourceView* imagePtr)
{
    return reinterpret_cast<ImTextureID>(imagePtr);
}
ImU32 toColor(int RGB, const int* A) {
    int R = (RGB >> 16) & 0xFF;
    int G = (RGB >> 8) & 0xFF;
    int B = RGB & 0xFF;
    int alpha = (A != nullptr) ? *A : 255;
    return (static_cast<ImU32>(alpha) << 24) |
        (static_cast<ImU32>(B) << 16) |
        (static_cast<ImU32>(G) << 8) |
        static_cast<ImU32>(R);
}