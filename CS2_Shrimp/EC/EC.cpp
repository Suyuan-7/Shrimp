#include "../usermode.h"
// ��ȡϵͳDPI������
float GetSystemDPI()
{
	HDC hdc = NULL;
	int dpi_a = NULL, dpi_b = NULL,dpi=NULL;
	hdc = GetDC(0);
	dpi_a = GetDeviceCaps(hdc, 118) / GetDeviceCaps(hdc, 8) * 100;
	dpi_b = GetDeviceCaps(hdc, 88) / 96 * 100;
	ReleaseDC(NULL,hdc);
	if (dpi_a == 100) {
		dpi = dpi_b;
	}
	else if (dpi_b == 100) {
		dpi = dpi_a;
	}
	else if (dpi_a == dpi_b) {
		dpi = dpi_a;
	}
	else {
		dpi = 0;
	}
	return (float)dpi / 100;
}

// �����ַ�(wchar_t)ת��ΪUTF-8�ַ���
std::string WideToUTF8(const std::wstring& wstr) {
	if (wstr.empty()) return "";

	// ��ȡת��������Ļ�������С
	int utf8_size = WideCharToMultiByte(
		CP_UTF8,            // Ŀ����룺UTF-8
		0,                  // ת����־
		wstr.c_str(),       // �����ַ���
		-1,                 // �Զ����㳤��
		nullptr,            // ��ֱ�����
		0,                  // ��ѯ��������С
		nullptr, nullptr    // Ĭ���ַ�����
	);

	if (utf8_size == 0) {
		// ������
		DWORD error = GetLastError();
		throw std::runtime_error("WideCharToMultiByte failed with error: " + std::to_string(error));
	}

	// ִ��ʵ��ת��
	std::string utf8_str(utf8_size, '\0');
	WideCharToMultiByte(
		CP_UTF8, 0,
		wstr.c_str(), -1,
		&utf8_str[0], utf8_size,
		nullptr, nullptr
	);

	// �Ƴ�ĩβ��null��ֹ��
	utf8_str.resize(utf8_size - 1);
	return utf8_str;
}

HWND FindCS2Window() noexcept {
    constexpr std::wstring_view classNames[] = { L"SDL_app", L"SDL_app" };
    constexpr std::wstring_view windowTitles[] = { L"Counter-Strike 2", L"���־�Ӣ��ȫ����" };

    for (auto className : classNames) {
        for (auto title : windowTitles) {
            if (HWND hWnd = FindWindowW(className.data(), title.data())) {
                return hWnd;
            }
        }
    }
    return nullptr;
}