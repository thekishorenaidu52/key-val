#ifndef SERVER_LOGGER_HPP
#define SERVER_LOGGER_HPP
#include <iostream>
#include <fstream>
#include <string>

namespace Server {

    class Logger {
    private:
        static std::ofstream logFile;

    public:
        static int init(const std::string& filename);
        static void log(const std::string& msg);
        static void close();
    };
} // namespace Server

#endif // SERVER_LOGGER_HPP
