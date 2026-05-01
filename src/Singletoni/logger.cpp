#include <Singletoni/logger.h>
#include <chrono>
#include <filesystem>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <cctype>
#include <string>

using namespace std;

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
    const std::string log_dir = "logs";
    std::filesystem::create_directories(log_dir);
    std::string filename = log_dir + "/" + current_date + ".log";
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
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

const char *level_to_color(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "\033[32m";
        case LogLevel::WARNING: return "\033[33m";
        case LogLevel::ERROR:   return "\033[38;2;255;0;0m";
        case LogLevel::DEBUG:   return "\033[36m";
        case LogLevel::FATAL:   return "\033[35m";
        default:                return "\033[0m";
    }
}

Logger& Logger::get_instance() {
    static Logger instance;
    return instance;
}

static std::string colorize_verdicts_in_message(const std::string& message) {
    const char *reset = "\033[0m";
    std::string res = message;

    const std::vector<std::pair<std::string, const char*>> map = {
        {"OK", "\033[32m"},
        {"PA", "\033[33m"},
        {"SKIP", "\033[36m"},
        {"CPE", "\033[35m"},
        {"WA", "\033[38;2;255;0;0m"},
        {"TLE", "\033[33m"},
        {"MLE", "\033[33m"},
        {"RTE", "\033[38;2;255;0;0m"},
        {"FAIL", "\033[38;2;255;0;0m"},
        {"ILE", "\033[33m"},
        {"NONE", "\033[38;5;123m"},
        {"OTHER", "\033[36m"}
    };

    for (const auto &p : map) {
        const std::string &token = p.first;
        const std::string color = p.second;
        const std::string colored_token = color + token + reset;

        size_t pos = 0;
        while ((pos = res.find(token, pos)) != std::string::npos) {
            bool boundary_before = (pos == 0) || !std::isalnum((unsigned char)res[pos - 1]);
            size_t after_pos = pos + token.size();
            bool boundary_after = (after_pos >= res.size()) || !std::isalnum((unsigned char)res[after_pos]);

            if (boundary_before && boundary_after) {
                res.replace(pos, token.size(), colored_token);
                pos += colored_token.size();
            } else {
                pos += token.size();
            }
        }
    }

    return res;
}

void Logger::log(LogLevel level, const char* file, int line, const std::string& message) {
    pthread_mutex_lock(&mtx);

    check_rotation();

    std::string timestamp_str = get_only_timestamp();
    const char *level_str = level_to_string(level);
    const char *color = level_to_color(level);
    const char *reset = "\033[0m";
    char level_with_brackets[32];
    snprintf(level_with_brackets, sizeof(level_with_brackets), "[%s]", level_str);

    // aici poate schimbam cu thread_id din context, nu stiu exact cum se comporta gettid
    unsigned long thread_id = (unsigned long)gettid();

    if (log_file != nullptr) {
        fprintf(log_file, "[%s] %-9s [TID:%6ld] [%s:%d] %s\n", timestamp_str.c_str(), level_with_brackets, thread_id, file, line, message.c_str());
        fflush(log_file);
    }
    std::string stdout_message = colorize_verdicts_in_message(message);
    fprintf(stdout, "[%s] %s%-9s%s [TID:%6ld] [%s:%d] %s\n", timestamp_str.c_str(), color, level_with_brackets, reset, thread_id, file, line, stdout_message.c_str());
    fflush(stdout);

    pthread_mutex_unlock(&mtx);
}

void Logger::log(LogLevel level, const char* file, int line, long long user_id, const std::string& message) {
    char uid_with_padding[32];
    snprintf(uid_with_padding, sizeof(uid_with_padding), "[UID:%2lld]", user_id);
    const std::string message_with_user = std::string(uid_with_padding) + " " + message;
    log(level, file, line, message_with_user);
}