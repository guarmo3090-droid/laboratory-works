#include "Logger.hpp"

using namespace std;

Logger::Logger() : minLevel(LogLevel::INFO) {
    logFile.open("application.log", ios::app);
    if (!logFile.is_open()) {
        cerr << "CRITICAL ERROR: Could not open application.log" << endl;
    }
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::setMinLevel(LogLevel level) {
    lock_guard<mutex> lock(logMutex);
    minLevel = level;
}

string Logger::getTimestamp() {
    auto now = chrono::system_clock::now();
    auto timeT = chrono::system_clock::to_time_t(now);
    tm tmStart;
    
    localtime_r(&timeT, &tmStart); 

    stringstream ss;
    ss << put_time(&tmStart, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const string& message) {
    lock_guard<mutex> lock(logMutex);

    if (level < minLevel) return;

    string timestamp = getTimestamp();
    string levelStr = getLevelString(level);

    stringstream fullMessage;
    fullMessage << "[" << timestamp << "] [" << levelStr << "] " << message;

    cout << fullMessage.str() << endl;
    
    if (logFile.is_open()) {
        logFile << fullMessage.str() << endl;
        // flush гарантує, що дані запишуться на диск, навіть якщо програма впаде
        logFile.flush(); 
    }
}
