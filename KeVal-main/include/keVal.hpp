#ifndef SERVER_KEVAL_HPP
#define SERVER_KEVAL_HPP
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "./constants/config.hpp"
#include "./constants/epollExitCodes.hpp"
#include "./keVal/store.hpp"
#include "./aof.hpp"
#include <sys/epoll.h>

namespace Server {
    namespace KeVal {
        void run_server();
        void process_client_message(int client_fd, const std::string& message, Store& store, AOF& aof);
        void setup_sigchld_handler(Store& store);

    } // namespace KeVal
} // namespace Server

#endif