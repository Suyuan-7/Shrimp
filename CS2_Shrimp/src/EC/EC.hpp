#pragma once
using namespace std;
namespace fs = std::filesystem;
namespace sys
{
    static std::string ExeFontDir;
    static std::string SysFontDir;
    inline BOOL debug = false;
    inline BOOL norefresh = false;//禁止DMA内部刷新
    inline BOOL ReadType = false;//假:WINAPI		真:DMA
    inline BOOL loop = false;//主要的线程循环条件
    inline uint64_t 剩余时间 = NULL;
    inline HWND hWnd = NULL;//窗口句柄
    inline DWORD dwPID = NULL;
    inline float dpi = NULL;
    inline uint32_t ScreenX = NULL;
    inline uint32_t ScreenY = NULL;
    inline uint32_t SizeX = NULL;
    inline uint32_t SizeY = NULL;
    inline BOOL 主界面状态 = NULL;
    inline BOOL 垂直同步 = NULL;
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
    // 游戏状态
    struct GameState {
        bool is_in_match = false;//在比赛中
        uint32_t current_round = 0;//当前回合
        uint32_t cached_round = 0; // 记忆回合
        uint32_t player_count = 0;//玩家数量
        uint32_t spectator_count = 0;//观众数量
    };
    // 玩家状态
    struct PlayerState {
        uint32_t local_player_index = 0;    // 本人索引
        uint32_t locked_target_index = 0;   // 锁定敌人索引
        float nearest_target_distance = 0.0f;//最近目标距离
        uint32_t aim_target_part = 0;       // 瞄准部位
        bool is_scoped = false;              // 开镜状态
    };
    // 瞄准数据
    struct AimData {
        ImVec2 punch_angles{ 0.0f, 0.0f };    // AimPunchAngle
        ImVec2 eye_angles{ 0.0f, 0.0f };      // AngEyeAngles
        ImVec2 recoil{ 0.0f, 0.0f };          // 反冲
        float sensitivity = 0.0f;
        ULONGLONG last_aim_trigger_time = 0;  // 上次自瞄扳机时间
        uint32_t trigger_index = 0;          // TriggerIndex
    };
    // 核心数据
    std::string map_name;
    uint64_t trigger_player = 0;  // TriggerPlayer
    // 结构化数据实例
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
