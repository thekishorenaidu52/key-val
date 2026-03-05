#ifndef SERVER_KEVAL_SERIALIZATIONPROTOCOL_HPP
#define SERVER_KEVAL_SERIALIZATIONPROTOCOL_HPP

#include <string>
#include "store.hpp"

namespace Server {
    namespace KeVal {
        // For both AOF and Snapshot serialization protocols
        // This class will handle the serialization and deserialization of data
        class FileIOSerializationProtocol {
        public:
            static std::string serializeKeyValue(const std::pair<const std::string, std::pair<std::string, u_int64_t>>& ptr);
            static std::string convertToLengthPrefixedString(const std::string& str, size_t length_prefix_size);
            static void serializeAndWrite(Store& store, std::ofstream& outFile);

            // Deserialize a string from a file
            static std::string deserialize(const std::string& filePath);

            // Check if the file exists
            static bool fileExists(const std::string& filePath);
        };
    }
}

#endif