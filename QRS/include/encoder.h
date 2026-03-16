#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include <vector>

class Encoder {
public:
    static void encodeFile(const std::string& inputFile, const std::string& outputDir);

private:
    static std::vector<uint8_t> readFileToMemory(const std::string& filePath);
    static std::vector<std::vector<uint8_t>> splitIntoChunks(const std::vector<uint8_t>& data, size_t chunkSize);
    static std::vector<uint8_t> createPacket(const std::vector<uint8_t>& chunk, size_t totalChunks, size_t chunkIndex, size_t origSize, const std::string& fileName);
    static void generateQRCode(const std::vector<uint8_t>& data, const std::string& outputPath, size_t chunkIndex);
};

#endif // ENCODER_H