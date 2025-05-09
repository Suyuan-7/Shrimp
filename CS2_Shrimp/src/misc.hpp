#pragma once
// 标准头文件保护
#ifndef IMVEC3_H
#define IMVEC3_H

// 内存对齐控制（兼容图形API要求）
#pragma pack(push, 1)

struct ImVec3 {
    // 成员变量
    float x, y, z;  // 单精度浮点数

    // ---- 构造函数 ----
    // 默认构造函数（零初始化）
    constexpr ImVec3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}

    // 标量构造函数（所有分量相同）
    explicit constexpr ImVec3(float scalar) noexcept
        : x(scalar), y(scalar), z(scalar) {
    }

    // 分量构造函数
    constexpr ImVec3(float _x, float _y, float _z) noexcept
        : x(_x), y(_y), z(_z) {
    }

    // ---- 运算符重载 ----
    // 加法
    ImVec3 operator+(const ImVec3& rhs) const noexcept {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    // 减法
    ImVec3 operator-(const ImVec3& rhs) const noexcept {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    // 标量乘法
    ImVec3 operator*(float scalar) const noexcept {
        return { x * scalar, y * scalar, z * scalar };
    }

    // 点积
    float dot(const ImVec3& rhs) const noexcept {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    // ---- 实用函数 ----
    // 向量长度
    float length() const noexcept {
        return std::sqrt(x * x + y * y + z * z);
    }

    // 归一化
    ImVec3 normalized() const noexcept {
        const float len = length();
        return (len > 0.0f) ? (*this) * (1.0f / len) : ImVec3();
    }
};

#pragma pack(pop) // 恢复默认对齐

#endif 
std::string ToUTF8(const wchar_t* wideStr);
// IMVEC3_H
std::string ToUTF8(const std::wstring& wideStr);
std::string ToUTF8(const char* text, UINT codePage);
std::string ToUTF8(const std::string& text, UINT codePage);
HWND FindCS2Window() noexcept;
std::string GetSystemFontDirectory();
std::string GetExecutableDirectory();

namespace conf 
{
	/* 本地字体目录 */
	extern std::string ExeFontDir;
	/* 系统字体 */
	extern std::string SysFontDir;
	/* 根目录 */
	extern std::string Dir_Root;
	/* 配置项目录 */
	extern std::string Dir_config;
}

namespace Colors {
    extern const int 黑色;
    extern const int 白色;
    extern const int 红色;
    extern const int 绿色;
    extern const int 蓝色;
    extern const int 黄色;
    extern const int 青色;
    extern const int 品红;
    extern const int 紫色;
    extern const int 橙色;
    extern const int 灰色;
    extern const int 浅灰;
    extern const int 深灰;
    extern const int 天蓝;
    extern const int 粉红;
    extern const int 金色;
    extern const int 银白;
    extern const int 茶色;
    extern const int 青绿;
    extern const int 透明色;
}