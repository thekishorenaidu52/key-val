#include "keVal.hpp"
#include "daemon.hpp"
#include "logger.hpp"
#include "keVal/store.hpp"
#include "aof.hpp"
#include <sstream>
#include <chrono>

namespace Server {
    namespace KeVal {
        // Function to process client messages

        void build_from_aof(Store& store, AOF& aof) {
            aof.read(store); 
        }

        void process_client_message(int client_fd, const std::string& message, Store& store, AOF& aof) {
            std::istringstream iss(message);
            std::string command, key, value, expiry;

            iss >> command >> key >> value >> expiry;
            std::string response;

            if (command == "SET") {
                if (expiry.empty()) {
                    store.set(key, value);
                } else {
                    store.set(key, value, std::stoull(expiry));
                }
                auto now = std::chrono::system_clock::now();
                auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                aof.append(std::to_string(now_ms) + " " + message);
                response = "OK";
            } else if (command == "GET") {
                response = store.get(key);
                
            } else if (command == "SNAPSHOT") {
                if (store.startSnapshot()) {
                    response = "SNAPSHOT_STARTED";
                }
            }else if(command == "EXISTS"){
                if(key.empty()){
                    response = "Error: Invalid EXISTS Usage";
                }else{
                    bool result = store.exists(key);
                    response = result ? "1": "0";
                }
            }
            else {
                response = "ERROR: Unknown command";
            }

            send(client_fd, response.c_str(), response.size() + 1, 0);
        }
        void run_server() {
            int server_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (server_fd < 0) {
                Logger::log("Socket creation failed");
                exit(1);
            }

            sockaddr_in address{};
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(Constants::Config::PORT);

            if(bind(server_fd, (struct sockaddr *)& address, sizeof(address)) < 0) {
                Logger::log("Bind Failed");
                close(server_fd);
                exit(1);
            }

            Logger::log("Server bound to port "+ std::to_string(Constants::Config::PORT));

            if(listen(server_fd, Constants::Config::MAX_CONNECTIONS) < 0) {
                Logger::log("Listen Failed");
                close(server_fd);
                exit(1);
            }

            Logger::log("Server is listening.....");

            Daemon::set_nonblocking(server_fd);

            int epoll_fd = epoll_create1(0);

            if (epoll_fd < 0) {
                Logger::log("Epoll creation failed !!!");
                close(server_fd);
                exit(Constants::EpollExitCodes::EPOLL_CREATE_FAILED);
            }

            epoll_event event{};
            event.events = EPOLLIN;
            event.data.fd = server_fd;

            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) < 0){
                Logger::log("Epoll control failed");
                close(server_fd);
                close(epoll_fd);
                exit(Constants::EpollExitCodes::EPOLL_CTL_ADD_FAILED);
            }

            Logger::log("Epoll is set up and server is ready to accept connections");
            Store store = Server::KeVal::Store();
            setup_sigchld_handler(store);
            Server::AOF aof(Constants::Config::AOF_PATH);
            build_from_aof(store, aof); // Load data from AOF into store
            

            epoll_event events[Constants::Config::MAX_EVENTS];

            while(true) {
               int event_count = epoll_wait(epoll_fd, events, Constants::Config::MAX_EVENTS, 100);

                for (int i = 0; i < event_count; ++i) {

                    if (events[i].data.fd == server_fd) {
                        while(true) {
                            sockaddr_in client_addr{};
                            socklen_t addrlen = sizeof(client_addr);
                            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);

                            if (client_fd < 0) {
                                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                    break; // No more pending connections
                                } else {
                                    Logger::log("Accept failed");
                                    break;
                                }
                            }

                            Daemon::set_nonblocking(client_fd);

                            epoll_event client_event{};
                            client_event.events = EPOLLIN | EPOLLET; // Edge-triggered
                            client_event.data.fd = client_fd;

                            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) < 0) {
                                Logger::log("Failed to add client to epoll");
                                close(client_fd);
                                continue;
                            }

                            Logger::log("Accepted new client: FD " + std::to_string(client_fd));
                        }
                    }
                    else {
                        // Handle client events
                        int client_fd = events[i].data.fd;
                        if (events[i].events & EPOLLIN) {
                            // Read from client
                            char buffer[1024];
                            while (true) {
                                ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
                                if (bytes_read > 0) {
                                    buffer[bytes_read] = '\0';
                                    std::string message(buffer);
                                    Logger::log("Received data from client FD " + std::to_string(client_fd));
                                    Logger::log("Data: " + message);
                                    message.erase(message.find_last_not_of(" \n\r\t") + 1); // Trim trailing whitespace

                                    process_client_message(client_fd, message, store, aof);
                                }
                                else if (bytes_read == 0) {
                                    Logger::log("Client disconnected: FD " + std::to_string(client_fd));
                                    close(client_fd);
                                    break;
                                }
                                else {
                                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                                        break; // No more data to read now
                                    else {
                                        Logger::log("Read error, closing client FD " + std::to_string(client_fd));
                                        close(client_fd);
                                        break;
                                    }
                                }
                            }
                        }
                        else if (events[i].events & EPOLLERR) {
                            // Handle error
                            Logger::log("Epoll error on client FD " + std::to_string(client_fd));
                            close(client_fd);
                        }
                        else if (events[i].events & EPOLLRDHUP) {
                            // Handle client disconnect
                            Logger::log("Client disconnected: FD " + std::to_string(client_fd));
                            close(client_fd);
                        }
                        else {
                            Logger::log("Unknown event for client FD " + std::to_string(client_fd));
                        }
                    }
                }

                if (store.snapshotDone.load()) {
                    store.snapshotDone.store(false);
                    int status;
                    pid_t pid = waitpid(store.snapshottingChildPid.load(), &status, WNOHANG);
                    if (pid > 0) {
                        Logger::log("Background snapshot finished. Exit code: " + std::to_string(WEXITSTATUS(status)));
                        store.snapshotting.store(false);
                    }
                }

                // TODO: active expiry goes here
            }
        }
    }
}