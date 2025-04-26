#pragma once
using namespace std;
namespace fs = std::filesystem;
namespace sys
{
    static std::string ExeFontDir;
    static std::string SysFontDir;
    inline BOOL debug = false;
    inline BOOL norefresh = false;//��ֹDMA�ڲ�ˢ��
    inline BOOL ReadType = false;//��:WINAPI		��:DMA
    inline BOOL loop = false;//��Ҫ���߳�ѭ������
    inline uint64_t ʣ��ʱ�� = NULL;
    inline HWND hWnd = NULL;//���ھ��
    inline DWORD dwPID = NULL;
    inline float dpi = NULL;
    inline uint32_t ScreenX = NULL;
    inline uint32_t ScreenY = NULL;
    inline uint32_t SizeX = NULL;
    inline uint32_t SizeY = NULL;
    inline BOOL ������״̬ = NULL;
    inline BOOL ��ֱͬ�� = NULL;
    inline ImFont* default_18 = nullptr;
    inline ImFont* default_22 = nullptr;
    inline ImFont* default_28 = nullptr;
    inline ImFont* default_48 = nullptr;
    inline ImFont* stratum2_18 = nullptr;
    inline ImFont* stratum2_32 = nullptr;
    inline ImFont* stratum2_64 = nullptr;
    inline ImFont* Quartz = nullptr;
    inline ImFont* Weaponioc = nullptr;
}

namespace imp
{
    void addText(ImVec2 pos, ImU32 col, std::wstring chinese_text);
    inline uint64_t engineBase = 0;
    inline uint64_t clientBase = 0;
	inline nlohmann::json Weapon = {};
	inline nlohmann::json Config = {};
}

namespace local_game_data
{
    // ��Ϸ״̬
    struct GameState {
        bool is_in_match = false;//�ڱ�����
        uint32_t current_round = 0;//��ǰ�غ�
        uint32_t cached_round = 0; // ����غ�
        uint32_t player_count = 0;//�������
        uint32_t spectator_count = 0;//��������
    };
    // ���״̬
    struct PlayerState {
        uint32_t local_player_index = 0;    // ��������
        uint32_t locked_target_index = 0;   // ������������
        float nearest_target_distance = 0.0f;//���Ŀ�����
        uint32_t aim_target_part = 0;       // ��׼��λ
        bool is_scoped = false;              // ����״̬
    };
    // ��׼����
    struct AimData {
        ImVec2 punch_angles{ 0.0f, 0.0f };    // AimPunchAngle
        ImVec2 eye_angles{ 0.0f, 0.0f };      // AngEyeAngles
        ImVec2 recoil{ 0.0f, 0.0f };          // ����
        float sensitivity = 0.0f;
        ULONGLONG last_aim_trigger_time = 0;  // �ϴ�������ʱ��
        uint32_t trigger_index = 0;          // TriggerIndex
    };
    // ��������
    std::string map_name;
    uint64_t trigger_player = 0;  // TriggerPlayer
    // �ṹ������ʵ��
    inline GameState game_state;
    inline PlayerState player_state;
    inline AimData aim_data;
}
std::string WideToUTF8(const std::wstring& wstr);
BOOL UpdateWindowData(HWND hWnd, HWND im_hWnd);
void menuloop();
void menu();
void objDraw();
HWND FindCS2Window() noexcept;
std::string GetSystemFontDirectory();
std::string GetExecutableDirectory();
