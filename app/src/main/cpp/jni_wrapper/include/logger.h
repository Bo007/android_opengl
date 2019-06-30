#pragma once

#include <string>
#include <cstdarg>

#define LOG_TAG "MyFirstGame"

#ifdef ANDROID

#include <android/log.h>

static void LogInfo(int priority, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    __android_log_vprint(priority, LOG_TAG, fmt, ap);
    va_end(ap);
}

#else
static const int ANDROID_LOG_ERROR = 0;
static const int ANDROID_LOG_WARN = 1;
static const int ANDROID_LOG_INFO = 2;
static const int ANDROID_LOG_DEBUG = 3;

static void LogInfo( int , const char *fmt, ...  )
{
    va_list ap;
    va_start( ap, fmt );
    std::string result = fmt;
    vprintf( ( result + '\n' ).c_str() , ap );
    va_end( ap );
}
#endif

template<typename ... Args>
static void LOGE(const std::string &fmt,
                 Args const &... args) noexcept {
    LogInfo(ANDROID_LOG_ERROR, fmt.c_str(), args ...);
}

template<typename ... Args>
static void LOGW(const std::string &fmt,
                 Args const &... args) noexcept {
    LogInfo(ANDROID_LOG_WARN, fmt.c_str(), args ...);
}

template<typename ... Args>
static void LOGI(const std::string &fmt,
                 Args const &... args) noexcept {
    LogInfo(ANDROID_LOG_INFO, fmt.c_str(), args ...);
}

template<typename ... Args>
static void LOGD(const std::string &fmt,
                 Args const &... args) noexcept {
    LogInfo(ANDROID_LOG_DEBUG, fmt.c_str(), args ...);
}
