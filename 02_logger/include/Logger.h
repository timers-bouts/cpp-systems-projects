#pragma once

#include <fstream>
#include <string>

// Log severity levels
enum class LogLevel {
    Info,
    Warn,
    Error
};

class Logger {
    private:
        std::ofstream file;
        LogLevel min_level;

        // Helper: returns formatted timestamp string
        std::string make_timestamp() const;

        // Helper: convert level to numeric severity
        static int severity(LogLevel level);

        // Core logging function (does filtering + formatting)
        void log(const std::string& message, LogLevel level);

    public:
        // Constructor: opens log file
        Logger(const std::string& filename, LogLevel minLevel);

        // Destructor: closes file automatically
        ~Logger();

        // Public API convenience methods
        void info(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);

        // Helper: convert enum to string label
        static const char* to_string(LogLevel level);
};
