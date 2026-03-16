#include <iostream>
#include <string>
#include "encoder.h"
#include "decoder.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage:\n";
        std::cout << "  Encode: QRGenerate.exe encode <input_file> <output_dir>\n";
        std::cout << "  Decode: QRGenerate.exe decode <input_dir>\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "encode") {
        if (argc != 4) {
            std::cout << "Encode usage: QRGenerate.exe encode <input_file> <output_dir>\n";
            return 1;
        }
        std::string inputFile = argv[2];
        std::string outputDir = argv[3];
        Encoder::encodeFile(inputFile, outputDir);
    } else if (mode == "decode") {
        if (argc != 3) {
            std::cout << "Decode usage: QRGenerate.exe decode <input_dir>\n";
            return 1;
        }
        std::string inputDir = argv[2];
        Decoder::decodeFiles(inputDir);
    } else {
        std::cout << "Invalid mode. Use 'encode' or 'decode'.\n";
        return 1;
    }

    return 0;
}