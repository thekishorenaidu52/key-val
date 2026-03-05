#include "daemon.hpp"
#include "logger.hpp"

namespace Server {
    void Daemon::fork_process(){
        pid_t pid = fork();
        if (pid < 0) {
            exit(Constants::DaemonExitCodes::FORK_FAILED);
        }
        if (pid > 0) {
            int logger_init = Logger::init(Constants::Config::LOG_FILE_PATH);
            
            if(logger_init == 1) {
                Logger::log("Daemon process started. PID: " + std::to_string(pid));
            }
        
            Logger::close();
            exit(0);
        }

        if (setsid() < 0) {
            exit(Constants::DaemonExitCodes::SETSID_FAILED);
        }

        if (chdir("/") < 0) {
            exit(Constants::DaemonExitCodes::CHDIR_FAILED);
        }

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        int logger_init = Logger::init(Constants::Config::LOG_FILE_PATH);
        if (logger_init == 0) {
            exit(Constants::DaemonExitCodes::LOG_FILE_OPEN_FAILED);
        }
    }


    void Daemon::set_nonblocking(int fd){
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}