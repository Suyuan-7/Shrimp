#pragma once

std::string WideToUTF8(const std::wstring& wstr);
HWND FindCS2Window() noexcept;
std::string GetSystemFontDirectory();
std::string GetExecutableDirectory();

namespace conf 
{
	/* 本地字体目录 */
	static std::string ExeFontDir;
	/* 系统字体 */
	static std::string SysFontDir;
	/* 根目录 */
	static std::string Dir_Root;
	/* 配置项目录 */
	static std::string Dir_config;
	/* 图片目录 */
	static std::string Dir_image;
}