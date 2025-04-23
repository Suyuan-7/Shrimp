#include "pch.hpp"
// 将宽字符(wchar_t)转换为UTF-8字符串
std::string WideToUTF8(const std::wstring& wstr) {
	if (wstr.empty()) return "";

	// 获取转换后所需的缓冲区大小
	int utf8_size = WideCharToMultiByte(
		CP_UTF8,            // 目标编码：UTF-8
		0,                  // 转换标志
		wstr.c_str(),       // 输入字符串
		-1,                 // 自动计算长度
		nullptr,            // 不直接输出
		0,                  // 查询缓冲区大小
		nullptr, nullptr    // 默认字符处理
	);

	if (utf8_size == 0) {
		// 错误处理
		DWORD error = GetLastError();
		throw std::runtime_error("WideCharToMultiByte failed with error: " + std::to_string(error));
	}

	// 执行实际转换
	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(
		CP_UTF8, 0,
		wstr.c_str(), -1,
		&utf8_str[0], utf8_size,
		nullptr, nullptr
	);

	// 移除末尾的null终止符
	utf8_str.resize(utf8_size - 1);
	return utf8_str;
}

HWND FindCS2Window() noexcept {
    constexpr std::wstring_view classNames[] = { L"SDL_app", L"SDL_app" };
    constexpr std::wstring_view windowTitles[] = { L"Counter-Strike 2", L"反恐精英：全球攻势" };

    for (auto className : classNames) {
        for (auto title : windowTitles) {
            if (HWND hWnd = FindWindowW(className.data(), title.data())) {
                return hWnd;
            }
        }
    }
    return nullptr;
}