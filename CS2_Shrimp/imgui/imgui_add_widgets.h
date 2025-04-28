#pragma once

namespace ImGui {

    IMGUI_API void Textfill(const char* text, const char* text_end = NULL);
    IMGUI_API bool TabButton(const char* label, const ImVec2& size_arg, bool active, ImFont* font = 0, const char* icon = 0, ImU32 icon_col = 0);
    IMGUI_API bool ToggleButton(const char* label, bool* v, const ImVec2& size_arg = {0,0});
    IMGUI_API bool HotKey(const char* label, unsigned int* pValue, const ImVec2& size_arg, const char* arrKeyNames[]);
    IMGUI_API bool ImageButtonEx(const char* label, ImTextureID pValue[], const ImVec2& size_arg, bool disabled, ImGuiButtonFlags flags);
    IMGUI_API bool SliderScalarO(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
    IMGUI_API bool SliderFloatO(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool SliderIntO(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
}