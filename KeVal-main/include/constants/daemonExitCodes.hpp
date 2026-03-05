#ifndef SERVER_CONSTANTS_DAEMON_EXIT_CODES_HPP
#define SERVER_CONSTANTS_DAEMON_EXIT_CODES_HPP
namespace Server {
    namespace Constants {  
        namespace DaemonExitCodes {
            constexpr int FORK_FAILED = 1; // Fork failed
            constexpr int SETSID_FAILED = 2; // setsid() failed
            constexpr int CHDIR_FAILED = 3; // chdir("/") failed
            constexpr int LOG_FILE_OPEN_FAILED = 4; // Log file open failed
            constexpr int EPOLL_CREATE_FAILED = 5; // epoll_create1() failed
            constexpr int EPOLL_CTL_ADD_FAILED = 6; // epoll_ctl() failed for adding server socket
            constexpr int ACCEPT_FAILED = 7; // accept() failed
            constexpr int EPOLL_WAIT_FAILED = 8; // epoll_wait() failed
        }
    }
}
#endif // SERVER_CONSTANTS_DAEMON_EXIT_CODES_HPP