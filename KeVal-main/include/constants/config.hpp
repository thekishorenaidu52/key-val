#ifndef SERVER_CONSTANTS_CONFIG_HPP
#define SERVER_CONSTANTS_CONFIG_HPP

namespace Server {
    namespace Constants {
        namespace Config {
            constexpr int PORT = 8080; // Default port for the server
            constexpr int BUFFER_SIZE = 1024; // Size of the buffer for reading data
            constexpr int MAX_EVENTS = 10; // Maximum number of events to handle in epoll_wait
            constexpr int MAX_CONNECTIONS = 10;
            constexpr const char* LOG_FILE_PATH = "/tmp/baddieCore_server.log"; // Path to the log file
            constexpr const char* AOF_PATH = "/tmp/baddieCore.aof"; // Path to the Append-Only File
        }
    }
}

#endif