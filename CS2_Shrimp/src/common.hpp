#pragma once
/* current build of cs2_webradar */
#define CS2_VERSION "2.0.430"

/* game modules */
#define CLIENT_DLL "client.dll"
#define ENGINE2_DLL "engine2.dll"

/* custom defines */
#define LOG_INFO(str, ...) \
    printf("[%s] [info] " str "\n", __TIME__,__VA_ARGS__)
#define LOG_WARNING(str, ...) \
    printf("[%s] [warning] " str "\n", __TIME__,__VA_ARGS__)
#define LOG_ERROR(str, ...) \
    { \
        const auto filename = std::filesystem::path(__FILE__).filename().string(); \
        printf("[%s] [error] " str "\n", filename.c_str(), __TIME__, __VA_ARGS__); \
        std::this_thread::sleep_for(std::chrono::seconds(5)); \
    }