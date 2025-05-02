#pragma once

ID3D11ShaderResourceView* LoadTextureFromFile(ID3D11Device* device, const char* filename);
bool InitDirectX(HWND hwnd);
void Cleanup();