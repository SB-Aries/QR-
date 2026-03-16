#include "decoder.h"
#include "fileutils.h"
#include "qrutils.h"
#include "imageutils.h"
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

void Decoder::decodeFiles(const string& inputDir) {
    // 查找所有Qr*.png文件
    vector<string> qrFiles = findQRFiles(inputDir);
    if (qrFiles.empty()) {
        cout << "No QR code files found in " << inputDir << endl;
        return;
    }
    
    // 解码每个QR码文件
    vector<pair<size_t, vector<uint8_t>>> chunks;
    size_t totalChunks = 0;
    size_t origSize = 0;
    string fileName;
    
    for (const string& filePath : qrFiles) {
        // 解码QR码
        vector<uint8_t> packet = decodeQRCode(filePath);
        if (packet.empty()) {
            cout << "Failed to decode " << filePath << endl;
            continue;
        }
        
        // 解析数据包
        size_t chunkIndex;
        vector<uint8_t> payload;
        parsePacket(packet, totalChunks, chunkIndex, origSize, fileName, payload);
        
        // 存储数据块
        chunks.emplace_back(chunkIndex, payload);
        cout << "Decoded chunk " << chunkIndex << " from " << filePath << endl;
    }
    
    // 按chunkIndex排序
    sort(chunks.begin(), chunks.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    
    // 重组文件
    if (!chunks.empty()) {
        reassembleFile(chunks, origSize, fileName);
        cout << "Decoding completed. Reassembled file: " << fileName << endl;
    } else {
        cout << "No valid chunks decoded." << endl;
    }
}

vector<string> Decoder::findQRFiles(const string& inputDir) {
    vector<string> files;
    for (const auto& entry : fs::directory_iterator(inputDir)) {
        if (entry.is_regular_file()) {
            string fileName = entry.path().filename().string();
            if (fileName.size() >= 8 && fileName.substr(0, 2) == "Qr" && fileName.substr(fileName.size() - 4) == ".png") {
                files.push_back(entry.path().string());
            }
        }
    }
    return files;
}

vector<uint8_t> Decoder::decodeQRCode(const string& filePath) {
    // 使用ZXing解码QR码
    using namespace ZXing;
    
    // 创建QR码读取器
    Reader reader;
    reader.setEanAddOnSymbolMode(Reader::EanAddOnSymbolMode::Ignore);
    
    // 读取图像文件
    auto image = ReadImage(filePath);
    if (!image.isValid()) {
        cout << "Failed to read image: " << filePath << endl;
        return vector<uint8_t>();
    }
    
    // 解码QR码
    auto result = reader.read(image);
    if (!result.isValid()) {
        cout << "Failed to decode QR code: " << filePath << endl;
        return vector<uint8_t>();
    }
    
    // 获取解码后的数据
    string decodedStr = result.text();
    vector<uint8_t> data(decodedStr.begin(), decodedStr.end());
    
    return data;
}

void Decoder::parsePacket(const vector<uint8_t>& packet, size_t& totalChunks, size_t& chunkIndex, size_t& origSize, string& fileName, vector<uint8_t>& payload) {
    if (packet.size() < 18) {
        return;
    }
    
    // 读取magic
    uint32_t magic = *reinterpret_cast<const uint32_t*>(packet.data());
    if (magic != QRUtils::MAGIC) {
        return;
    }
    
    // 读取totalChunks
    totalChunks = *reinterpret_cast<const uint32_t*>(packet.data() + 4);
    
    // 读取chunkIndex
    chunkIndex = *reinterpret_cast<const uint32_t*>(packet.data() + 8);
    
    // 读取origSize
    origSize = *reinterpret_cast<const uint32_t*>(packet.data() + 12);
    
    // 读取nameLen
    uint16_t nameLen = *reinterpret_cast<const uint16_t*>(packet.data() + 16);
    
    // 读取name
    if (packet.size() >= 18 + nameLen) {
        fileName.assign(packet.begin() + 18, packet.begin() + 18 + nameLen);
        
        // 读取payload
        payload.assign(packet.begin() + 18 + nameLen, packet.end());
    }
}

void Decoder::reassembleFile(const vector<pair<size_t, vector<uint8_t>>>& chunks, size_t origSize, const string& fileName) {
    vector<uint8_t> fileData;
    
    // 拼接所有数据块
    for (const auto& chunk : chunks) {
        fileData.insert(fileData.end(), chunk.second.begin(), chunk.second.end());
    }
    
    // 截断到原始大小
    if (fileData.size() > origSize) {
        fileData.resize(origSize);
    }
    
    // 写入文件
    FileUtils::writeFile(fileName, fileData);
}