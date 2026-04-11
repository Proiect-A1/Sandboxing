#include <Singletoni/logger.h>
#include <chrono>
#include <sys/time.h>

Logger* Logger::instance = nullptr;
pthread_mutex_t Logger::mtx = PTHREAD_MUTEX_INITIALIZER;

Logger::Logger() : log_file(nullptr) {
    open_log_file();
}

Logger::~Logger() {
    if (log_file != nullptr) {
        fclose(log_file);
    }
}

std::string Logger::get_only_date() {
    time_t now = time(0);
    struct tm tstruct;
    localtime_r(&now, &tstruct);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return std::string(buf);
}

std::string Logger::get_only_timestamp() {
    auto now = std::chrono::system_clock::now();
    time_t in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    struct tm time_info;
    localtime_r(&in_time_t, &time_info);

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &time_info);

    char final_buffer[120];
    snprintf(final_buffer, sizeof(final_buffer), "%s.%03d", buffer, (int)ms.count());

    return std::string(final_buffer);
}

void Logger::open_log_file() {
    if (log_file != nullptr) {
        fclose(log_file);
    }
    
    current_date = get_only_date();
    std::string filename = "/logs/" + current_date + ".log";
    
    log_file = fopen(filename.c_str(), "a");
    if (log_file != nullptr) {
        setvbuf(log_file, NULL, _IONBF, 0);
    }
}

void Logger::check_rotation() {
    std::string today = get_only_date();
    if (today != current_date) {
        open_log_file();
    }
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
    pthread_mutex_lock(&mtx);
    
    check_rotation();

    if (log_file != nullptr) {
        std::string timestamp_str = get_only_timestamp();
        const char* level_str = level_to_string(level);
        unsigned long thread_id = (unsigned long)pthread_self();

        fprintf(log_file, "[%s] [%s] [Th-%lu] [%s:%d] %s\n", 
            timestamp_str.c_str(), level_str, thread_id, file, line, message.c_str());
        fflush(log_file);
    }
    
    pthread_mutex_unlock(&mtx);
}