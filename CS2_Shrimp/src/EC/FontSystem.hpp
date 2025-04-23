#pragma once
#include <string>

namespace FontSys {
        struct Fonts {
            ImFont* default_18 = nullptr;  ///< 默认字体-微软雅黑18pt (中文字符集)
            ImFont* default_22 = nullptr;  ///< 默认字体-微软雅黑粗体22pt
            ImFont* default_28 = nullptr;  ///< 默认字体-微软雅黑粗体28pt
            ImFont* default_48 = nullptr;  ///< 默认字体-微软雅黑粗体48pt
            ImFont* digital2_18 = nullptr;  ///< Stratum2科技字体18pt
            ImFont* digital2_32 = nullptr;  ///< Stratum2科技字体32pt 
            ImFont* digital2_64 = nullptr;  ///< Stratum2科技字体64pt
            ImFont* quartz_32 = nullptr;    ///< Quartz风格字体32pt
            ImFont* icon_28 = nullptr;      ///< 软件图标字体28pt
            ImFont* weapon_64 = nullptr;    ///< 武器数字字体64pt
    };
        bool Initialize(Fonts & fonts, float dpi_scale = 1.0f);
}