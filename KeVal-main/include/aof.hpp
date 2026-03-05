#ifndef SERVER_AOF_HPP
#define SERVER_AOF_HPP

#include <string>
#include <fstream>
#include "./keVal/store.hpp"

namespace Server {
    class AOF {
    public:
        explicit AOF(const std::string& filename);
        ~AOF();

        void append(const std::string& data);
        void read(KeVal::Store& store);

    private:
        std::string filename;
        std::fstream aof_file;
    };
}

#endif // SERVER_AOF_HPP
