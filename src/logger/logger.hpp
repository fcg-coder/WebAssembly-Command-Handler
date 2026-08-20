#include <emscripten.h>

// Уровни логирования
#define LOG_ERR_LVL 1
#define LOG_WRN_LVL 2
#define LOG_INF_LVL 3
#define LOG_DBG_LVL 4

#define CURRENT_LOG_LVL 3

// Функция для вызова console.log из C++
EM_JS(void, js_console_log, (const char* level, const char* file, int line, const char* message), {
    console.log(UTF8ToString(level) + ' ' + UTF8ToString(file) + ':' + line + ': ' + UTF8ToString(message));
});

// Базовый макрос для логирования
#define LOG(level, level_str, fmt, ...) \
    do { \
        if (level <= CURRENT_LOG_LVL) { \
            char buffer[512]; \
            snprintf(buffer, sizeof(buffer), fmt, ##__VA_ARGS__); \
            js_console_log(level_str, __FILE__, __LINE__, buffer); \
        } \
    } while(0)

// Макросы для каждого уровня
#define LOG_ERR(fmt, ...) LOG(LOG_ERR_LVL, "ERROR", fmt, ##__VA_ARGS__)
#define LOG_WRN(fmt, ...) LOG(LOG_WRN_LVL, "WARN",  fmt, ##__VA_ARGS__)
#define LOG_INF(fmt, ...) LOG(LOG_INF_LVL, "INFO",  fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt, ...) LOG(LOG_DBG_LVL, "DEBUG", fmt, ##__VA_ARGS__)

// Упрощенные версии без форматирования
#define LOG_ERR_MSG(msg) LOG(LOG_ERR_LVL, "ERROR", "%s", msg)
#define LOG_WRN_MSG(msg) LOG(LOG_WRN_LVL, "WARN",  "%s", msg)
#define LOG_INF_MSG(msg) LOG(LOG_INF_LVL, "INFO",  "%s", msg)
#define LOG_DBG_MSG(msg) LOG(LOG_DBG_LVL, "DEBUG", "%s", msg)