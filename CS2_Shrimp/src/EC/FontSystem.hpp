#pragma once
#include <string>

namespace FontSys {
        struct Fonts {
            ImFont* default_18 = nullptr;  ///< Ĭ������-΢���ź�18pt (�����ַ���)
            ImFont* default_22 = nullptr;  ///< Ĭ������-΢���źڴ���22pt
            ImFont* default_28 = nullptr;  ///< Ĭ������-΢���źڴ���28pt
            ImFont* default_48 = nullptr;  ///< Ĭ������-΢���źڴ���48pt
            ImFont* digital2_18 = nullptr;  ///< Stratum2�Ƽ�����18pt
            ImFont* digital2_32 = nullptr;  ///< Stratum2�Ƽ�����32pt 
            ImFont* digital2_64 = nullptr;  ///< Stratum2�Ƽ�����64pt
            ImFont* quartz_32 = nullptr;    ///< Quartz�������32pt
            ImFont* icon_28 = nullptr;      ///< ���ͼ������28pt
            ImFont* weapon_64 = nullptr;    ///< ������������64pt
    };
        bool Initialize(Fonts & fonts, float dpi_scale = 1.0f);
}