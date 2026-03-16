#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <string>
#include <vector>

class ImageUtils {
public:
    static void saveQRAsPNG(const std::vector<bool>& qrMatrix, int size, const std::string& outputPath);
    static std::vector<bool> loadPNGAndDecode(const std::string& filePath);
};

#endif // IMAGEUTILS_H