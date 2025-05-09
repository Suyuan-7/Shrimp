#pragma once
// ��׼ͷ�ļ�����
#ifndef IMVEC3_H
#define IMVEC3_H

// �ڴ������ƣ�����ͼ��APIҪ��
#pragma pack(push, 1)

struct ImVec3 {
    // ��Ա����
    float x, y, z;  // �����ȸ�����

    // ---- ���캯�� ----
    // Ĭ�Ϲ��캯�������ʼ����
    constexpr ImVec3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}

    // �������캯�������з�����ͬ��
    explicit constexpr ImVec3(float scalar) noexcept
        : x(scalar), y(scalar), z(scalar) {
    }

    // �������캯��
    constexpr ImVec3(float _x, float _y, float _z) noexcept
        : x(_x), y(_y), z(_z) {
    }

    // ---- ��������� ----
    // �ӷ�
    ImVec3 operator+(const ImVec3& rhs) const noexcept {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    // ����
    ImVec3 operator-(const ImVec3& rhs) const noexcept {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    // �����˷�
    ImVec3 operator*(float scalar) const noexcept {
        return { x * scalar, y * scalar, z * scalar };
    }

    // ���
    float dot(const ImVec3& rhs) const noexcept {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    // ---- ʵ�ú��� ----
    // ��������
    float length() const noexcept {
        return std::sqrt(x * x + y * y + z * z);
    }

    // ��һ��
    ImVec3 normalized() const noexcept {
        const float len = length();
        return (len > 0.0f) ? (*this) * (1.0f / len) : ImVec3();
    }
};

#pragma pack(pop) // �ָ�Ĭ�϶���

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
	/* ��������Ŀ¼ */
	extern std::string ExeFontDir;
	/* ϵͳ���� */
	extern std::string SysFontDir;
	/* ��Ŀ¼ */
	extern std::string Dir_Root;
	/* ������Ŀ¼ */
	extern std::string Dir_config;
}

namespace Colors {
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int Ʒ��;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ��ɫ;
    extern const int ǳ��;
    extern const int ���;
    extern const int ����;
    extern const int �ۺ�;
    extern const int ��ɫ;
    extern const int ����;
    extern const int ��ɫ;
    extern const int ����;
    extern const int ͸��ɫ;
}