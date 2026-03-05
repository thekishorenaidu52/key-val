#include <iostream>
#include "daemon.hpp"
#include "keVal.hpp"
#include "logger.hpp"

int main(){
    Server::Daemon::fork_process();
    Server::KeVal::run_server();
}
