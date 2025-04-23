#include "pch.hpp"
#include <ShlObj.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace FontSys {
    namespace { // 内部实现细节
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
            config.FontDataOwnedByAtlas = false; // 防止重复释放

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

        // 清空旧字体集
        atlas.Clear();

        //============== 系统字体加载 ==============
        const auto sys_font_dir = GetSystemFontDir();

        // 微软雅黑常规18pt (带中文)
        fonts.default_18 = SafeLoadFont(&atlas,
            sys_font_dir + "msyh.ttc",
            18.0f * dpi_scale,
            atlas.GetGlyphRangesChineseFull()
        );
        if (!fonts.default_18) all_success = false;

        // 微软雅黑粗体22pt (带中文)
        fonts.default_22 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            22.0f * dpi_scale,
            atlas.GetGlyphRangesChineseFull()
        );
        if (!fonts.default_22) all_success = false;

        // 微软雅黑粗体28pt (默认字形)
        fonts.default_28 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            28.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // 微软雅黑粗体48pt
        fonts.default_48 = SafeLoadFont(&atlas,
            sys_font_dir + "msyhbd.ttc",
            48.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        //============= 应用字体加载 ==============
        const auto app_font_dir = GetAppFontDir();

        // Stratum2系列字体
        const auto stratum2_path = app_font_dir + "stratum2.ttf";
        fonts.digital2_18 = SafeLoadFont(&atlas, stratum2_path, 18.0f * dpi_scale, nullptr);
        fonts.digital2_32 = SafeLoadFont(&atlas, stratum2_path, 32.0f * dpi_scale, nullptr);
        fonts.digital2_64 = SafeLoadFont(&atlas, stratum2_path, 64.0f * dpi_scale, nullptr);

        // Quartz字体
        fonts.quartz_32 = SafeLoadFont(&atlas,
            app_font_dir + "Quartz.ttf",
            32.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // 图标字体
        fonts.icon_28 = SafeLoadFont(&atlas,
            app_font_dir + "tga.ttf",
            28.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // 武器数字字体
        fonts.weapon_64 = SafeLoadFont(&atlas,
            app_font_dir + "Weaponioc.ttf",
            64.0f * dpi_scale,
            atlas.GetGlyphRangesDefault()
        );

        // 构建字体集
        atlas.Build();
        return all_success;
    }
}