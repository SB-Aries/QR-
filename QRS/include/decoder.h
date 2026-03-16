#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>

class Decoder {
public:
    static void decodeFiles(const std::string& inputDir);

private:
    static std::vector<std::string> findQRFiles(const std::string& inputDir);
    static std::vector<uint8_t> decodeQRCode(const std::string& filePath);
    static void parsePacket(const std::vector<uint8_t>& packet, size_t& totalChunks, size_t& chunkIndex, size_t& origSize, std::string& fileName, std::vector<uint8_t>& payload);
    static void reassembleFile(const std::vector<std::pair<size_t, std::vector<uint8_t>>>& chunks, size_t origSize, const std::string& fileName);
};

#endif // DECODER_H