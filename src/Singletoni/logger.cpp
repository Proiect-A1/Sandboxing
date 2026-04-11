#include <Singletoni/logger.h>
#include <chrono>
#include <sys/time.h>

Logger* Logger::instance = nullptr;
pthread_mutex_t Logger::mtx = PTHREAD_MUTEX_INITIALIZER;

Logger::Logger() {
    log_file = fopen("/logs/central.log", "a");
    if (log_file != nullptr) {
        setvbuf(log_file, NULL, _IONBF, 0);
    }
}git 

Logger::~Logger() {
    if (log_file != nullptr) {
        fclose(log_file);
    }
}

std::string Logger::get_current_timestamp() {
    auto now = std::chrono::system_clock::now();
    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    struct tm time_info;
    localtime_r(&in_time_t, &time_info);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);

    char final_buffer[120];
    snprintf(final_buffer, sizeof(final_buffer), "%s.%03d", buffer, (int)ms.count());

    return std::string(final_buffer);
}

const char* Logger::level_to_string(LogLevel level) {
    switch(level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::DEBUG:   return "DEBUG";
        default:                return "OTHER";
    }
}

Logger& Logger::get_instance() {
    if (instance == nullptr) {
        pthread_mutex_lock(&mtx);
        if (instance == nullptr) {
            instance = new Logger();
        }
        pthread_mutex_unlock(&mtx);
    }
    return *instance;
}

void Logger::log(LogLevel level, const char* file, int line, const std::string& message) {
    std::string timestamp = get_current_timestamp();
    const char* level_str = level_to_string(level);
    
    unsigned long thread_id = (unsigned long)pthread_self();

    pthread_mutex_lock(&mtx);
    if (log_file != nullptr) {
        fprintf(log_file, "[%s] [%s] [Th-%lu] [%s:%d] %s\n", 
            timestamp.c_str(), level_str, thread_id, file, line, message.c_str());
        fflush(log_file);
    }
    pthread_mutex_unlock(&mtx);
}