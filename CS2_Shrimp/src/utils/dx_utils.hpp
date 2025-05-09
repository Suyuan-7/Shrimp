#pragma once
//ID3D11ShaderResourceView* LoadTextureFromFile(const char* filename);
ID3D11ShaderResourceView* LoadTextureImageFromMemory(const BYTE* data, size_t dataSize);
void ReleaseTextureResource(ID3D11ShaderResourceView* srv);
bool InitDirectX(HWND hwnd);
void Cleanup();
ImTextureID µΩÕº∆¨÷∏’Î(ID3D11ShaderResourceView* imagePtr);
ImU32 toColor(int RGB, const int* A = nullptr);