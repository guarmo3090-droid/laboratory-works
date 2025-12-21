#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& message);
    
    void trace(const std::string& message) { log(LogLevel::TRACE, message); }
    void debug(const std::string& message) { log(LogLevel::DEBUG, message); }
    void info(const std::string& message)  { log(LogLevel::INFO, message); }
    void warn(const std::string& message)  { log(LogLevel::WARN, message); }
    void error(const std::string& message) { log(LogLevel::ERROR, message); }

    void setMinLevel(LogLevel level);

private:
    Logger();
    ~Logger();

    std::string getTimestamp();
    std::string getLevelString(LogLevel level);

    std::mutex logMutex;
    std::ofstream logFile;
    LogLevel minLevel;
};
