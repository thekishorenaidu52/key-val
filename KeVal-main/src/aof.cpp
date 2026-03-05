#include "./aof.hpp"
#include "./logger.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace Server {

    AOF::AOF(const std::string& filename)
        : filename(filename), aof_file(filename, std::ios::in | std::ios::out | std::ios::app | std::ios::binary)
    {
        if (!aof_file.is_open()) {
            throw std::runtime_error("Failed to open AOF file: " + filename);
        }
    }

    AOF::~AOF() {
        if (aof_file.is_open()) {
            aof_file.close();
        }
    }

    void AOF::append(const std::string& data) {
        if (!aof_file.is_open()) return;
        aof_file.clear();  // Clear any fail bits
        aof_file.seekp(0, std::ios::end);
        aof_file << data << '\n';
        aof_file.flush();
    }

    void AOF::read(KeVal::Store& store) {
        if (!aof_file.is_open()) return;
        aof_file.clear();  // Clear any fail bits
        aof_file.seekg(0, std::ios::beg);

        std::string line;
        while (std::getline(aof_file, line)) {
            std::istringstream iss(line);
            std::string timestamp, command, key, value, expiry;

            iss >> timestamp >> command >> key >> value >> expiry;

            if (command == "SET") {
                if (expiry.empty()) {
                    store.set(key, value);
                } else {
                    store.set(key, value, std::stoull(expiry));
                }
            }
        }
    }

}
