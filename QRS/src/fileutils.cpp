#include "fileutils.h"
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

vector<uint8_t> FileUtils::readFile(const string& filePath) {
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file) {
        return vector<uint8_t>();
    }
    
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);
    
    vector<uint8_t> buffer(size);
    if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return buffer;
    }
    
    return vector<uint8_t>();
}

void FileUtils::writeFile(const string& filePath, const vector<uint8_t>& data) {
    ofstream file(filePath, ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
}

string FileUtils::getFileName(const string& filePath) {
    fs::path path(filePath);
    return path.filename().string();
}

void FileUtils::createDirectory(const string& dirPath) {
    fs::create_directories(dirPath);
}