#include "pch.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ID3D11ShaderResourceView* LoadTextureFromFile(ID3D11Device* device, const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
            if (!data) return nullptr;

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
            initData.pSysMem = data;
            initData.SysMemPitch = width * 4;
            initData.SysMemSlicePitch = width * height * 4;

            ID3D11Texture2D* texture = nullptr;
            HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
            stbi_image_free(data);
            if (FAILED(hr)) return nullptr;

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            ID3D11ShaderResourceView* srv = nullptr;
            hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
            texture->Release();

            if (FAILED(hr)) return nullptr;

            return srv;
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