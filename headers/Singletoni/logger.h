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
    FATAL
};

class Logger {
private:
    static Logger* instance;
    static pthread_mutex_t mtx;

    FILE* log_file;
    std::string current_date;

    Logger();
    
    std::string get_only_date();
    std::string get_only_timestamp();
    const char* level_to_string(LogLevel level);
    
    void open_log_file();
    void check_rotation();

public:
    static Logger& get_instance();
    ~Logger();
    void log(LogLevel level, const char* file, int line, const std::string& message);
    void log(LogLevel level, const char* file, int line, long long user_id, const std::string& message);
};

#define LOG_INFO(msg)    Logger::get_instance().log(LogLevel::INFO,    __FILE__, __LINE__, msg)
#define LOG_WARNING(msg) Logger::get_instance().log(LogLevel::WARNING, __FILE__, __LINE__, msg)
#define LOG_ERROR(msg)   Logger::get_instance().log(LogLevel::ERROR,   __FILE__, __LINE__, msg)
#define LOG_DEBUG(msg)   Logger::get_instance().log(LogLevel::DEBUG,   __FILE__, __LINE__, msg)
#define LOG_FATAL(msg)   Logger::get_instance().log(LogLevel::FATAL,   __FILE__, __LINE__, msg)

#define LOG_INFO_USER(user_id, msg)    Logger::get_instance().log(LogLevel::INFO,    __FILE__, __LINE__, user_id, msg)
#define LOG_WARNING_USER(user_id, msg) Logger::get_instance().log(LogLevel::WARNING, __FILE__, __LINE__, user_id, msg)
#define LOG_ERROR_USER(user_id, msg)   Logger::get_instance().log(LogLevel::ERROR,   __FILE__, __LINE__, user_id, msg)
#define LOG_DEBUG_USER(user_id, msg)   Logger::get_instance().log(LogLevel::DEBUG,   __FILE__, __LINE__, user_id, msg)
#define LOG_FATAL_USER(user_id, msg)   Logger::get_instance().log(LogLevel::FATAL,   __FILE__, __LINE__, user_id, msg)

#endif // LOGGER_H