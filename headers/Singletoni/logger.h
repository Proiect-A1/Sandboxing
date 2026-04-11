#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>
#include <cstdio>
#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG,
    OTHER
};

class Logger {
private:
    static Logger* instance;
    static pthread_mutex_t mtx;

    FILE* log_file;

    Logger();
    
    std::string get_current_timestamp();
    const char* level_to_string(LogLevel level);

public:
    static Logger& get_instance();
    ~Logger();
    void log(LogLevel level, const char* file, int line, const std::string& message);
};

#define LOG_INFO(msg)    Logger::get_instance().log(LogLevel::INFO,    __FILE__, __LINE__, msg)
#define LOG_WARNING(msg) Logger::get_instance().log(LogLevel::WARNING, __FILE__, __LINE__, msg)
#define LOG_ERROR(msg)   Logger::get_instance().log(LogLevel::ERROR,   __FILE__, __LINE__, msg)
#define LOG_DEBUG(msg)   Logger::get_instance().log(LogLevel::DEBUG,   __FILE__, __LINE__, msg)
#define LOG_OTHER(msg)   Logger::get_instance().log(LogLevel::OTHER,   __FILE__, __LINE__, msg)

#endif // LOGGER_H