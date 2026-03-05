#include "./keVal/serializationProtocol.hpp"
#include <fstream>
#include <stdexcept>

namespace Server {
    namespace KeVal {

        // void FileIOSerializationProtocol::serializeAndWrite(Store& store, std::ofstream& outFile) {
        //     // Serialize the store object to a file
        //     if (!outFile) {
        //         throw std::invalid_argument("Invalid output file stream");
        //     }
        // }

        std::string FileIOSerializationProtocol::convertToLengthPrefixedString(const std::string& str, size_t length_prefix_size) {
            if (length_prefix_size != 4 && length_prefix_size != 8) {
                throw std::invalid_argument("Length prefix size must be either 4 or 8 bytes");
            }
            std::string result;
            result.resize(length_prefix_size + str.size());

            // Write the length prefix
            if (length_prefix_size == 4) {
                uint32_t len = static_cast<uint32_t>(str.size());
                result[0] = (len >> 24) & 0xFF;
                result[1] = (len >> 16) & 0xFF;
                result[2] = (len >> 8) & 0xFF;
                result[3] = len & 0xFF;
            } else {
                uint64_t len = static_cast<uint64_t>(str.size());
                for (int i = 0; i < 8; ++i) {
                    result[i] = (len >> (56 - i * 8)) & 0xFF;
                }
            }

            // Copy the string data
            std::copy(str.begin(), str.end(), result.begin() + length_prefix_size);
            return result;  
        }
        std::string FileIOSerializationProtocol::serializeKeyValue(const std::pair<const std::string, std::pair<std::string, u_int64_t>>& keyValue) {
            const std::string& key = keyValue.first;
            const std::string& value = keyValue.second.first;
            uint64_t expiry = keyValue.second.second;

            std::string serialized;
            serialized += convertToLengthPrefixedString(key, 4); // 4 bytes for key length
            serialized += convertToLengthPrefixedString(value, 8); // 8 bytes for value length
            serialized.append(reinterpret_cast<const char*>(&expiry), sizeof(expiry));
            return serialized;
        }

        // std::string FileIOSerializationProtocol::deserialize(const std::string& filePath) {
        //     std::ifstream inFile(filePath, std::ios::binary);
        //     if (!inFile) {
        //         throw std::runtime_error("Failed to open file for reading: " + filePath);   
        //     }
        // }
    }
}