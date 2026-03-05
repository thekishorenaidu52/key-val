#ifndef SERVER_DAEMON_HPP
#define SERVER_DAEMON_HPP

#include <string>
#include <fcntl.h>
#include <csignal>
#include "./constants/daemonExitCodes.hpp"
#include "./constants/config.hpp"

namespace Server {
    namespace Daemon {

        // Function to run the server as a daemon
        void fork_process();

        // Function to set a file descriptor to non-blocking mode
        void set_nonblocking(int fd);

    } // namespace Daemon
}

#endif // SERVER_DAEMON_HPP