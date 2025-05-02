#pragma once

std::string WideToUTF8(const std::wstring& wstr);
HWND FindCS2Window() noexcept;
std::string GetSystemFontDirectory();
std::string GetExecutableDirectory();

namespace conf 
{
	/* ��������Ŀ¼ */
	static std::string ExeFontDir;
	/* ϵͳ���� */
	static std::string SysFontDir;
	/* ��Ŀ¼ */
	static std::string Dir_Root;
	/* ������Ŀ¼ */
	static std::string Dir_config;
	/* ͼƬĿ¼ */
	static std::string Dir_image;
}