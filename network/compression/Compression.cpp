// /*
// ** EPITECH PROJECT, 2023
// ** rtype
// ** File description:
// ** compression.cpp
// */

#include "Compression.hpp"

std::vector<uint8_t> RLE::compress(std::vector<uint8_t>& data)
{
    std::vector<uint8_t> compressed;
    uint8_t count = 1;
    uint8_t current = data[0];

    for (int i = 1; i < data.size(); i++) {
        if (data[i] == current && count < 255) {
            count++;
        } else {
            compressed.push_back(count);
            compressed.push_back(current);
            current = data[i];
            count = 1;
        }
    }
    compressed.push_back(count);
    compressed.push_back(current);
    return compressed;
}

std::array<uint8_t, 1024> RLE::decompress(std::array<uint8_t, 1024>& data)
{
    std::array<uint8_t, 1024> decompressed;
    int j = 0;

    for (int i = 0; i < data.size(); i += 2) {
        for (int k = 0; k < data[i]; k++) {
            decompressed[j] = data[i + 1];
            j++;
        }
    }
    return decompressed;
}

int main()
{
    RLE rle;
    std::vector<uint8_t> data = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 };
    for (int i = 0; i < data.size(); i++) {
        std::cout << (int)data[i] << std::endl;
    }
    std::cout << "----------------" << std::endl;
    std::vector<uint8_t> compressed = rle.compress(data);
    for (int i = 0; i < compressed.size(); i++) {
        std::cout << (int)compressed[i] << std::endl;
    }
    std::cout << "----------------" << std::endl;
    std::array<uint8_t, 1024> compressed2 = { 9, 1, 1, 2 };
    std::array<uint8_t, 1024> decompressed = rle.decompress(compressed2);
    std::cout << "size: " << decompressed.size() << std::endl;
    // for (int i = 0; i < decompressed.size(); i++) {
    //     std::cout << (int)decompressed[i] << std::endl;
    // }
    return 0;
}
