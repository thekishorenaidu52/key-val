#ifndef SERVER_CONATANTS_EPOLL_EXIT_CODES_HPP
#define SERVER_CONATANTS_EPOLL_EXIT_CODES_HPP
namespace Server {
    namespace Constants {
        namespace EpollExitCodes {
            constexpr int EPOLL_CREATE_FAILED = 1; // epoll_create1() failed
            constexpr int EPOLL_CTL_ADD_FAILED = 2; // epoll_ctl() failed for adding server socket
            constexpr int ACCEPT_FAILED = 3; // accept() failed
            constexpr int EPOLL_WAIT_FAILED = 4; // epoll_wait() failed
            constexpr int SET_NON_BLOCKING_FAILED = 5; // set_nonblocking() failed
        }
    }
}
#endif // SERVER_CONATANTS_EPOLL_EXIT_CODES_HPP