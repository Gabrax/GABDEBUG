#ifndef GABLOG_H
#define GABLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#ifndef GABLOG_ENABLE
    #define GABLOG_ENABLE 1
#endif

#ifndef GABLOG_UNSTRIP_PREFIX
    #define GABLOG_STRIP_PREFIX
#endif

typedef enum
{
    LOG_TRACE,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_CRITICAL
} LogLevel;

void Logger_SetLevel(LogLevel level);
void Logger_Log(LogLevel level, const char* file, int line, const char* fmt, ...);

#if GABLOG_ENABLE

    #define GABLOG_TRACE(...)    Logger_Log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
    #define GABLOG_INFO(...)     Logger_Log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
    #define GABLOG_WARN(...)     Logger_Log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
    #define GABLOG_ERROR(...)    Logger_Log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
    #define GABLOG_CRITICAL(...) Logger_Log(LOG_CRITICAL, __FILE__, __LINE__, __VA_ARGS__)

    #ifdef GABLOG_STRIP_PREFIX
        #define TRACE(...)    GABLOG_TRACE(__VA_ARGS__)
        #define INFO(...)     GABLOG_INFO(__VA_ARGS__)
        #define WARN(...)     GABLOG_WARN(__VA_ARGS__)
        #define ERROR(...)    GABLOG_ERROR(__VA_ARGS__)
        #define CRITICAL(...) GABLOG_CRITICAL(__VA_ARGS__)
    #endif

#else

    #define GABLOG_TRACE(...)    ((void)0)
    #define GABLOG_INFO(...)     ((void)0)
    #define GABLOG_WARN(...)     ((void)0)
    #define GABLOG_ERROR(...)    ((void)0)
    #define GABLOG_CRITICAL(...) ((void)0)

    #ifdef GABLOG_STRIP_PREFIX
        #define TRACE(...)    ((void)0)
        #define INFO(...)     ((void)0)
        #define WARN(...)     ((void)0)
        #define ERROR(...)    ((void)0)
        #define CRITICAL(...) ((void)0)
    #endif

#endif

#if defined(_MSC_VER)
    #define DEBUG_BREAK() __debugbreak()
#else
    #include <signal.h>
    #define DEBUG_BREAK() raise(SIGTRAP)
#endif

#if GABLOG_ENABLE

    #define GABLOG_ASSERT(x, fmt, ...)                                      \
    do {                                                                    \
        if (!(x)) {                                                         \
            Logger_Log(LOG_CRITICAL, __FILE__, __LINE__,                    \
                       "Assertion Failed: " fmt, ##__VA_ARGS__);             \
            DEBUG_BREAK();                                                  \
        }                                                                   \
    } while(0)

    #ifdef GABLOG_STRIP_PREFIX
        #define ASSERT(...) GABLOG_ASSERT(__VA_ARGS__)
    #endif

#else

    #define GABLOG_ASSERT(...) ((void)0)

    #ifdef GABLOG_STRIP_PREFIX
        #define ASSERT(...) ((void)0)
    #endif

#endif

#ifdef __cplusplus
}
#endif

#ifdef GABLOG_IMPLEMENTATION

#include <string.h>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <pthread.h>
    #include <unistd.h>
#endif

static LogLevel g_LogLevel = LOG_TRACE;
static int g_UseColor = 1;
static int g_TerminalInitialized = 0;

#if defined(_WIN32)
static CRITICAL_SECTION g_LogMutex;
static int g_MutexInitialized = 0;
#else
static pthread_mutex_t g_LogMutex = PTHREAD_MUTEX_INITIALIZER;
#endif


static void gablog_init_mutex(void)
{
#if defined(_WIN32)
    if (!g_MutexInitialized)
    {
        InitializeCriticalSection(&g_LogMutex);
        g_MutexInitialized = 1;
    }
#endif
}

static void gablog_lock(void)
{
#if defined(_WIN32)
    EnterCriticalSection(&g_LogMutex);
#else
    pthread_mutex_lock(&g_LogMutex);
#endif
}

static void gablog_unlock(void)
{
#if defined(_WIN32)
    LeaveCriticalSection(&g_LogMutex);
#else
    pthread_mutex_unlock(&g_LogMutex);
#endif
}

static void gablog_init_terminal(void)
{
#if defined(_WIN32)

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        g_UseColor = 0;
        return;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode))
    {
        g_UseColor = 0;
        return;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hOut, mode))
        g_UseColor = 0;

#else
    if (!isatty(fileno(stdout)))
        g_UseColor = 0;
#endif
}

static inline const char* gablog_filename(const char* path)
{
    const char* slash = strrchr(path, '/');

#if defined(_WIN32)
    const char* backslash = strrchr(path, '\\');
    if (!slash || (backslash && backslash > slash))
        slash = backslash;
#endif

    return slash ? slash + 1 : path;
}

static inline const char* LevelToString(LogLevel level)
{
    switch (level)
    {
        case LOG_TRACE:    return "TRACE";
        case LOG_INFO:     return "INFO ";
        case LOG_WARN:     return "WARN ";
        case LOG_ERROR:    return "ERROR";
        case LOG_CRITICAL: return "CRIT ";
        default:           return "UNKWN";
    }
}

static inline const char* LevelColor(LogLevel level)
{
    if (!g_UseColor)
        return "";

    switch (level)
    {
        case LOG_TRACE:    return "\033[37m";
        case LOG_INFO:     return "\033[32m";
        case LOG_WARN:     return "\033[33m";
        case LOG_ERROR:    return "\033[31m";
        case LOG_CRITICAL: return "\033[41m";
        default:           return "\033[0m";
    }
}

void Logger_SetLevel(const LogLevel level)
{
    g_LogLevel = level;
}

void Logger_Log(const LogLevel level, const char* file, int line, const char* fmt, ...)
{
#if GABLOG_ENABLE

    if (level < g_LogLevel)
        return;

    if (!g_TerminalInitialized)
    {
        gablog_init_mutex();
        gablog_init_terminal();
        g_TerminalInitialized = 1;
    }

    gablog_lock();

    char message[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    fprintf(stdout,
            "%s[%s] %s%s (%s:%d)\n",
            LevelColor(level),
            LevelToString(level),
            message,
            g_UseColor ? "\033[0m" : "",
            gablog_filename(file),
            line);

    fflush(stdout);

    gablog_unlock();

#endif
}

#endif /* GABLOG_IMPLEMENTATION */

#endif /* GABLOG_H */