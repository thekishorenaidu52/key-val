#ifndef SERVER_KEVAL_STORE_HPP
#define SERVER_KEVAL_STORE_HPP

#include <unordered_map>
#include <atomic>
#include <signal.h>
#include <string>
#include <sys/wait.h>
namespace Server {
    namespace KeVal {
        class Store {
            std::unordered_map<std::string, std::pair<std::string, u_int64_t>> keys;

            public:
                std::atomic<bool> snapshotting;
                std::atomic<pid_t> snapshottingChildPid;
                std::atomic<bool> snapshotDone;
                Store() : snapshotting(false), snapshottingChildPid(-1), snapshotDone(false) {}
                void set(const std::string& key, const std::string& value);
                void set(const std::string& key, const std::string& value, u_int64_t expiry);
                std::string get(const std::string& key);
                bool startSnapshot();
                bool exists(const std::string& key);
        };
        extern Store* storeInstance;
    }
}
#endif // SERVER_KEVAL_STORE_HPP