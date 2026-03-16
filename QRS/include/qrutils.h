#ifndef QRUTILS_H
#define QRUTILS_H

#include <vector>
#include <string>

class QRUtils {
public:
    static const size_t MAGIC = 0x51525354; // 'QRST'
    static const size_t CHUNK_PAYLOAD_MAX = 2953; // 版本40，ECC级别H的最大容量
    
    static std::vector<uint8_t> encodeData(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decodeData(const std::string& encoded);
};

#endif // QRUTILS_H