/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** compression Huffman
*/

#ifndef COMPRESSION_HPP
    #define COMPRESSION_HPP

    #include <vector>
    #include <array>
    #include <cstdint>
    #include <iostream>
    #include "zlib.h"

class RLE {
    public:
        RLE() = default;
        ~RLE() = default;
        std::vector<uint8_t> compress(std::vector<uint8_t> &data);
        std::array<uint8_t, 1024> decompress(std::array<uint8_t, 1024> &data);
};


#endif //COMPRESSION_HPP