#include "./keVal/store.hpp"
#include "./logger.hpp"
#include <chrono>

namespace Server {
    namespace KeVal {
        Store* storeInstance = nullptr;

        // Helper function to get current time in seconds since epoch
        static u_int64_t get_current_time() {
            return static_cast<u_int64_t>(
                std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()
                ).count()
            );
        }
        
        static u_int64_t get_expiry_time(u_int64_t expiry) {
            return get_current_time() + expiry; // Return the absolute expiry time
        }

        void Store::set(const std::string& key, const std::string& value) {
            keys[key] = make_pair(value, 0); // 0 indicates no expiry
        }

        void Store::set(const std::string& key, const std::string& value, u_int64_t expiry) {
            keys[key] = make_pair(value, get_expiry_time(expiry)); // expiry is in seconds
        }

        std::string Store::get(const std::string& key) {
            if (keys.find(key) != keys.end()) {
                auto [value, expiry] = keys[key];
            
                if (expiry == 0 || (get_current_time() < expiry)) {
                    return value;
                }
                else {
                    keys.erase(key);
                }
            }
            return "";
        }


        static void onChildExit(int) {
            Logger::log("SIGCHLD received");
            if (storeInstance) storeInstance->snapshotDone.store(true);
        }

        void setup_sigchld_handler(Store& store) {
            storeInstance = &store;
            struct sigaction sa{};
            sa.sa_handler = onChildExit;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            if (sigaction(SIGCHLD, &sa, nullptr) != 0) {
                perror("sigaction");
                exit(1);
            }
        }

        bool Store::startSnapshot() {
            bool expected = false;
            if (!snapshotting.compare_exchange_strong(expected, true)) {
                Logger::log("Snapshot already running.");
                return false;
            }

            pid_t pid = fork();
            if (pid == 0) {
                Logger::log("Child process: starting snapshot");
                
                Logger::log("Child process: snapshot complete");
                _exit(0);
            } else if (pid < 0) {
                Logger::log("Fork failed.");
                snapshotting.store(false);
                return false;
            } else {
                snapshottingChildPid.store(pid);
                Logger::log("Parent process: snapshot started with pid " + std::to_string(pid));
                return true;
            }
        }
        bool Store::exists(const std::string& key) {
            return keys.find(key) != keys.end();
        }
        
    }
}
