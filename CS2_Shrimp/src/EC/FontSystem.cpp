#include "pch.hpp"
#include <ShlObj.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace FontSys {
    namespace { // �ڲ�ʵ��ϸ��
            std::string GetSystemFontDir() {
            char path[MAX_PATH];
            SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, path);
            return std::string(path) + "\\";
        }
            std::string GetAppFontDir() {
            char exe_path[MAX_PATH];
            GetModuleFileNameA(nullptr, exe_path, MAX_PATH);
            fs::path p(exe_path);
            return (p.parent_path() / "fonts").string() + "\\";
        }       
            ImFont * SafeLoadFont(ImFontAtlas * atlas, const std::string & path,
                float size, const ImWchar * ranges) {
            if (!fs::exists(path)) return nullptr;

            ImFontConfig config;
            config.FontDataOwnedByAtlas = false; // ��ֹ�ظ��ͷ�

            return atlas->AddFontFromFileTTF(
                path.c_str(),
                size,
                &config,
                ranges
            );
        }
    }

    bool Initialize(Fonts& fonts, float dpi_scale) {
        auto& atlas = *ImGui::GetIO().Fonts;
        bool all_success = true;

        // ��վ����弯
        atlas.Clear();

        //============== ϵͳ������� ==============
        const auto sys_font_dir = GetSystemFontDir();

        // ΢���źڳ���18pt (������)
        fonts.default_18 = SafeLoadFont(&atlas,
            sys_font_dir + "msyh.ttc",
            18.0f * dpi_scale,
            atlas.GetGlyphRangesChineseFull()
        );
        if (!fonts.default_18) all_success = false;

        // ΢���źڴ���22pt (������)
        fonts.default_22 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            22.0f * dpi_scale,
            atlas.GetGlyphRangesChineseFull()
        );
        if (!fonts.default_22) all_success = false;

        // ΢���źڴ���28pt (Ĭ������)
        fonts.default_28 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            28.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // ΢���źڴ���48pt
        fonts.default_48 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            48.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        //============= Ӧ��������� ==============
        const auto app_font_dir = GetAppFontDir();

        // Stratum2ϵ������
        const auto stratum2_path = app_font_dir + "stratum2.ttf";
        fonts.digital2_18 = SafeLoadFont(&atlas, stratum2_path, 18.0f * dpi_scale, nullptr);
        fonts.digital2_32 = SafeLoadFont(&atlas, stratum2_path, 32.0f * dpi_scale, nullptr);
        fonts.digital2_64 = SafeLoadFont(&atlas, stratum2_path, 64.0f * dpi_scale, nullptr);

        // Quartz����
        fonts.quartz_32 = SafeLoadFont(&atlas,
            app_font_dir + "Quartz.ttf",
            32.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // ͼ������
        fonts.icon_28 = SafeLoadFont(&atlas,
            app_font_dir + "tga.ttf",
            28.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // ������������
        fonts.weapon_64 = SafeLoadFont(&atlas,
            app_font_dir + "Weaponioc.ttf",
            64.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // �������弯
        atlas.Build();
        return all_success;
    }
}