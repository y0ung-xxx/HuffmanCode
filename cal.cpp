#include <iostream>
#include <fstream>

// 计算压缩率
double calculateCompressionRatio(const std::string &originalFile, const std::string &compressedFile)
{
    std::ifstream original(originalFile, std::ifstream::binary | std::ifstream::ate);
    std::ifstream compressed(compressedFile, std::ifstream::binary | std::ifstream::ate);

    if (!original || !compressed)
    {
        std::cout << "Error opening input files" << std::endl;
        return 0.0;
    }

    // 获取文件大小
    double originalFileSize = static_cast<double>(original.tellg());
    double compressedFileSize = static_cast<double>(compressed.tellg());

    // 计算压缩率
    double compressionRatio = (compressedFileSize / originalFileSize) * 100.0;

    original.close();
    compressed.close();

    return compressionRatio;
}

int main()
{
    // std::string originalFile = "test01.txt";
    // std::string compressedFile = "test01.txt.huffman";
    // std::string originalFile = "test02.txt";
    // std::string compressedFile = "test02.txt.huffman";
    std::string originalFile = "test03.txt";
    std::string compressedFile = "test03.txt.huffman";
    
    double compressionRatio = calculateCompressionRatio(originalFile, compressedFile);
    std::cout << "Compression Ratio: " << compressionRatio << "%" << std::endl;
    return 0;
}