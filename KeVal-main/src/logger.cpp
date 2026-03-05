#include "logger.hpp"

namespace Server {

    std::ofstream Logger::logFile;

    int Logger::init(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
            return 0; // Return an error code if the file cannot be opened
        }
        logFile << "Logger initialized. Log file: " << filename << std::endl;
        return 1; // Return 0 on success
    }

    void Logger::log(const std::string& msg) {
        if (logFile.is_open()) {
            logFile << msg << std::endl;
        }
    }

    void Logger::close() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

} // namespace Server
