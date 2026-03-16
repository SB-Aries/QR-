#include "encoder.h"
#include "fileutils.h"
#include "qrutils.h"
#include "imageutils.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

void Encoder::encodeFile(const string& inputFile, const string& outputDir) {
    // 读取文件到内存
    vector<uint8_t> fileData = readFileToMemory(inputFile);
    
    // 计算总块数
    size_t totalChunks = (fileData.size() + QRUtils::CHUNK_PAYLOAD_MAX - 1) / QRUtils::CHUNK_PAYLOAD_MAX;
    
    // 创建输出目录
    FileUtils::createDirectory(outputDir);
    
    // 分割数据块并生成QR码
    vector<vector<uint8_t>> chunks = splitIntoChunks(fileData, QRUtils::CHUNK_PAYLOAD_MAX);
    for (size_t i = 0; i < chunks.size(); ++i) {
        // 创建数据包
        vector<uint8_t> packet = createPacket(chunks[i], totalChunks, i, fileData.size(), FileUtils::getFileName(inputFile));
        
        // 生成QR码并保存为PNG
        stringstream ss;
        ss << outputDir << "\\Qr" << setw(4) << setfill('0') << i << ".png";
        generateQRCode(packet, ss.str(), i);
        
        cout << "Generated QR code: " << ss.str() << endl;
    }
    
    cout << "Encoding completed. Generated " << chunks.size() << " QR codes." << endl;
}

vector<uint8_t> Encoder::readFileToMemory(const string& filePath) {
    return FileUtils::readFile(filePath);
}

vector<vector<uint8_t>> Encoder::splitIntoChunks(const vector<uint8_t>& data, size_t chunkSize) {
    vector<vector<uint8_t>> chunks;
    size_t pos = 0;
    while (pos < data.size()) {
        size_t remaining = data.size() - pos;
        size_t currentSize = min(chunkSize, remaining);
        chunks.emplace_back(data.begin() + pos, data.begin() + pos + currentSize);
        pos += currentSize;
    }
    return chunks;
}

vector<uint8_t> Encoder::createPacket(const vector<uint8_t>& chunk, size_t totalChunks, size_t chunkIndex, size_t origSize, const string& fileName) {
    vector<uint8_t> packet;
    
    // 写入magic
    uint32_t magic = QRUtils::MAGIC;
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&magic), reinterpret_cast<uint8_t*>(&magic) + 4);
    
    // 写入totalChunks
    uint32_t tc = static_cast<uint32_t>(totalChunks);
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&tc), reinterpret_cast<uint8_t*>(&tc) + 4);
    
    // 写入chunkIndex
    uint32_t ci = static_cast<uint32_t>(chunkIndex);
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&ci), reinterpret_cast<uint8_t*>(&ci) + 4);
    
    // 写入origSize
    uint32_t os = static_cast<uint32_t>(origSize);
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&os), reinterpret_cast<uint8_t*>(&os) + 4);
    
    // 写入nameLen
    uint16_t nl = static_cast<uint16_t>(fileName.size());
    packet.insert(packet.end(), reinterpret_cast<uint8_t*>(&nl), reinterpret_cast<uint8_t*>(&nl) + 2);
    
    // 写入name
    packet.insert(packet.end(), fileName.begin(), fileName.end());
    
    // 写入payload
    packet.insert(packet.end(), chunk.begin(), chunk.end());
    
    return packet;
}

void Encoder::generateQRCode(const vector<uint8_t>& data, const string& outputPath, size_t chunkIndex) {
    // 使用ZXing生成QR码
    using namespace ZXing;
    
    // 创建QR码写入器
    Writer writer(BarcodeFormat::QRCode);
    writer.setEncoding(CharacterSet::BINARY);
    writer.setErrorCorrectionLevel(ErrorCorrectionLevel::H); // 使用最高纠错级别
    
    // 将二进制数据转换为字符串
    string dataStr(reinterpret_cast<const char*>(data.data()), data.size());
    
    // 生成QR码
    auto result = writer.encode(dataStr);
    if (!result.isValid()) {
        cout << "Failed to generate QR code" << endl;
        return;
    }
    
    // 获取QR码大小和数据
    int size = result.width();
    vector<bool> qrMatrix(size * size);
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            qrMatrix[y * size + x] = result.get(x, y);
        }
    }
    
    // 保存为PNG
    ImageUtils::saveQRAsPNG(qrMatrix, size, outputPath);
}