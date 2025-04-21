

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_add_widgets.h"

#include <unordered_map>

static constexpr const char* arrKeyNames[] =
{
    "",
    "mouse left", "mouse right", "cancel", "mouse middle", "mb 1", "mb2", "",
    "backspace", "tab", "", "", "clear", "enter", "", "",
    "shift", "control", "alt", "pause", "caps", "", "", "", "", "", "",
    "escape", "", "", "", "", "space", "page up", "page down",
    "end", "home", "left", "up", "right", "down", "", "", "",
    "print", "insert", "delete", "",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    "", "", "", "", "", "", "",
    "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k",
    "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
    "v", "w", "x", "y", "z", "lwin", "rwin", "", "", "",
    "num0", "num1", "num2", "num3", "num4", "num5",
    "num6", "num7", "num8", "num9",
    "*", "+", "", "-", ".", "/",
    "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8",
    "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16",
    "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24",
    "", "", "", "", "", "", "", "",
    "num lock", "scroll lock",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", "",
    "lshift", "rshift", "lctrl",
    "rctrl", "lmenu", "rmenu",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", "",
    "", "", "", "", "", "~", "",
    "", "", "", "", "", "", 

};


void ImGui::Textfill(const char* text, const char* text_end)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    ImGuiContext& g = *GImGui;

    // Accept null ranges
    if (text == text_end)
        text = text_end = "";

    // Calculate length
    const char* text_begin = text;
    if (text_end == NULL)
        text_end = text + strlen(text); // FIXME-OPT

    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    if (text_end - text <= 2000 || wrap_enabled)
    {
        // Common case
        const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
        const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

        ImRect bb(text_pos, text_pos + text_size);
        ItemSize(text_size, 0.0f);
        if (!ItemAdd(bb, 0))
            return;

        window->DrawList->AddRectFilled(bb.Min - ImVec2(5.0f, 2.0f), bb.Max + ImVec2(5.0f, 2.0f), GetColorU32(ImGuiCol_FrameBg), g.Style.FrameRounding);

        // Render (we don't hide text after ## in this end-user function)
        RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
    }
}

bool ImGui::TabButton(const char* label, const ImVec2& size_arg, bool active, ImFont* font, const char* icon, ImU32 icon_col)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_None);

    struct Animation {
        ImColor text;
        ImColor Frame;
    };

    static std::unordered_map< ImGuiID, Animation > animation;
    if (animation.find(id) == animation.end()) {
        animation.insert({ id, Animation() });
    }

    animation[id].Frame = ImLerp(
        animation[id].Frame.Value,
        GetStyleColorVec4((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button),
        g.IO.DeltaTime * 8.f);

    animation[id].text = ImLerp(
        animation[id].text.Value,
        GetStyleColorVec4(active ? ImGuiCol_Text : ImGuiCol_TextDisabled),
        g.IO.DeltaTime * 8.f);

    // Render
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, animation[id].Frame, true, style.FrameRounding);


    // Icon
    if (font) {
        PushFont(font);
        ImVec2 icon_size = CalcTextSize(icon);
        window->DrawList->AddText(bb.Min + ImVec2(10, bb.GetSize().y * 0.5f - icon_size.y * 0.5f), icon_col == 0 ? animation[id].text : ImColor(icon_col), icon);
        PopFont();
    }

    // Label
    window->DrawList->AddText(bb.Min + ImVec2(bb.GetSize().x * 0.5f - label_size.x * 0.5f , bb.GetSize().y * 0.5f - label_size.y * 0.5f), animation[id].text, label);

    return pressed;
}

bool ImGui::ToggleButton(const char* label, bool* v, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGuiID id = g.CurrentWindow->GetID(label);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight() * 0.8f;
    float width = height * 2.5f;

    ImVec2 size = CalcItemSize(size_arg, width + style.FramePadding.x * 2.0f, height + style.FramePadding.y * 2.0f);

    ImGui::InvisibleButton(label, ImVec2(size));
    if (ImGui::IsItemClicked()) 
        *v = !*v;

    struct Animation {
        ImColor Frame;
        float CirclePos;
        Animation() : Frame(0,0,0,0), CirclePos(0.0f){ }
    };

    static std::unordered_map< ImGuiID, Animation > animation;
    if (animation.find(id) == animation.end())
        animation.insert({ id, Animation()});


    float radius = size.y * 0.50f;

    animation[id].Frame = ImLerp(animation[id].Frame.Value, *v ? GetStyleColorVec4(ImGuiCol_SwitchOpenBg) : GetStyleColorVec4(ImGuiCol_SwitchCloseBg), g.IO.DeltaTime * 8.f);
    animation[id].CirclePos = ImLerp(animation[id].CirclePos, *v ? (size.x - radius * 2.0f) : 0.0f, g.IO.DeltaTime * 8.f);

    draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), animation[id].Frame, size.y * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + animation[id].CirclePos, p.y + radius), radius - 4.0f, ImGui::GetColorU32(*v ? GetStyleColorVec4(ImGuiCol_SwitchOpenGrab) : GetStyleColorVec4(ImGuiCol_SwitchCloseGrab)));
   
    const float border_size = g.Style.FrameBorderSize;
    if (border_size > 0.0f) {
        draw_list->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), GetColorU32(ImGuiCol_Border), size.y * 0.50f);
    }

    return *v;
}

bool ImGui::HotKey(const char* szLabel, unsigned int* pValue, const ImVec2& size_arg, const char* keyMap[])
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    if (window->SkipItems)
        return false;

    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(szLabel);
    const ImVec2 label_size = CalcTextSize(szLabel, nullptr, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;
    
    bool hovered = ItemHoverable(bb, id, g.LastItemData.InFlags);
    const bool bClicked = hovered && io.MouseClicked[0];
    const bool bDoubleClicked = hovered && io.MouseDoubleClicked[0];
    if (bClicked || bDoubleClicked)
    {
        if (g.ActiveId != id)
        {
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *pValue = 0U;
        }

        SetActiveID(id, window);
    }

    bool bValueChanged = false;

    unsigned int nKey = *pValue;

    if (g.ActiveId == id)
    {
        for (int n = 0; n < 5; n++)
        {
            if (IsMouseDown(n))
            {
                switch (n)
                {
                case 0:
                    nKey = 1;
                    break;
                case 1:
                    nKey = 2;
                    break;
                case 2:
                    nKey = 4;
                    break;
                case 3:
                    nKey = 5;
                    break;
                case 4:
                    nKey = 6;
                    break;
                }

                bValueChanged = true;
                ClearActiveID();
            }
        }

        if (!bValueChanged)
        {
            for (int n = 0; n < 200; n++)
            {
                if (IsKeyDown((ImGuiKey)n))
                {
                    nKey = n;
                    bValueChanged = true;
                    ClearActiveID();
                }
            }
        }

        if (IsKeyPressed(ImGuiKey_Escape))
        {
            *pValue = 0U;
            ClearActiveID();
        }
        else
            *pValue = nKey;
    }



    char szBuffer[64] = { };
    char* szBufferEnd = &szBuffer[0];
    if (*pValue != 0 && g.ActiveId != id)
        strcat_s(szBufferEnd, 64, keyMap[*pValue]);
    else if (g.ActiveId == id)
        strcat_s(szBufferEnd, 64, "press a key");
    else
        strcat_s(szBufferEnd, 64, "none");

    const ImVec2 new_label_size = CalcTextSize(szBufferEnd, nullptr, true);

    struct Animation {
        ImColor text;
        ImColor Frame;
    };
    static std::unordered_map< ImGuiID, Animation > animation;
    if (animation.find(id) == animation.end()) {
        animation.insert({ id, Animation() });
    }
    animation[id].Frame = ImLerp(
        animation[id].Frame.Value,
        GetStyleColorVec4(hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button),
        g.IO.DeltaTime * 8.f);

    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, animation[id].Frame, true, style.FrameRounding);
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, szBufferEnd, NULL, &new_label_size, style.ButtonTextAlign, &bb);

    return bValueChanged;
}

bool ImGui::ImageButtonEx(const char* label, ImTextureID pValue[], const ImVec2& image_size, bool disabled, ImGuiButtonFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);
    const ImVec2 padding = g.Style.FramePadding;
    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + image_size + padding * 2.0f);
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    // Render
    ImTextureID  texture_id = 0;

    if (disabled) {
        
        texture_id = pValue[3];

    }
    else {
        texture_id = (held && hovered) ? pValue[2] : hovered ? pValue[1] : pValue[0];

    }

    window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding);

    return pressed;
}

bool ImGui::SliderScalarO(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = CalcItemWidth();

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max /*+ ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f)*/);

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    bool temp_input_is_active = temp_input_allowed && TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool clicked = hovered && IsMouseClicked(0, id);
        const bool make_active = (clicked || g.NavActivateId == id);
        if (make_active && clicked)
            SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if ((clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
            SetActiveID(id, window);
            SetFocusID(id, window);
            FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    // Draw frame
    const ImU32 frame_col = GetColorU32(g.ActiveId == id ? /*ImGuiCol_FrameBgActive : hovered ?*/ ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    RenderNavHighlight(frame_bb, id);

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    RenderSliderFrame(grab_bb, frame_bb, frame_col, true, g.Style.FrameRounding);

    //-------------------------------------------------------------------------
    float percent = floorf((float)(*(float*)p_data - *(float*)p_min) / (float)(*(float*)p_max - *(float*)p_min) * 100.f);
    struct SliderAnimation { float position; float alpha; };
    static std::unordered_map<ImGuiID, SliderAnimation> animation;
    if (animation.find(id) == animation.end()) {
        animation.insert({ id, SliderAnimation() });
    }
    animation[id].position = ImLerp(animation[id].position, g.Style.Alpha < 0.5f ? 0.f : percent, g.IO.DeltaTime * 12.f);
    animation[id].alpha = ImLerp(animation[id].alpha, hovered ? 1.f : 0.0f, g.IO.DeltaTime * 12.f);

    float r = grab_bb.GetHeight() * 0.5f;
    ImVec2 grab_pos = ImVec2(frame_bb.Min.x + r + (frame_bb.GetSize().x - r * 2.0f) * animation[id].position * 0.01f, grab_bb.GetCenter().y);
    
    {
        ImRect slider_bb = frame_bb;
        slider_bb.Min.x += r;
        slider_bb.Min.y += r;
        slider_bb.Max.x -= r;
        slider_bb.Max.y -= r;
        
        window->DrawList->AddRectFilled(slider_bb.Min, slider_bb.Max, frame_col, g.Style.FrameRounding);
        window->DrawList->AddRectFilled(slider_bb.Min, ImVec2(grab_pos.x, slider_bb.Max.y), GetColorU32(ImGuiCol_FrameBgActive), g.Style.FrameRounding);
        window->DrawList->AddCircleFilled(grab_pos, r * 0.8f, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab));
    }

    if (animation[id].alpha > 0.0f) {
        
        char value_buf[64];
        DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

        const ImVec2 value_size = CalcTextSize(value_buf, NULL, true);
        const ImVec2 value_pos = ImVec2(grab_pos.x - value_size.x * 0.5f, grab_pos.y - value_size.y * 2.0f);
        const ImRect bb = ImRect(value_pos, value_pos + value_size);

        window->DrawList->AddRectFilled(bb.Min - ImVec2(2.0f, 1.0f), bb.Max + ImVec2(2.0f, 1.0f), GetColorU32(ImGuiCol_FrameBg, animation[id].alpha), g.Style.FrameRounding);
        window->DrawList->AddText(value_pos, GetColorU32(ImGuiCol_Text, animation[id].alpha), value_buf);
    }



    return value_changed;
}

bool ImGui::SliderFloatO(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    return SliderScalarO(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
}

bool ImGui::SliderIntO(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return SliderScalarO(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
}




