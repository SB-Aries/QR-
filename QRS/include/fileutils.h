#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>

class FileUtils {
public:
    static std::vector<uint8_t> readFile(const std::string& filePath);
    static void writeFile(const std::string& filePath, const std::vector<uint8_t>& data);
    static std::string getFileName(const std::string& filePath);
    static void createDirectory(const std::string& dirPath);
};

#endif // FILEUTILS_H