/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include <iostream>
#include <asio.hpp>
#include "src/socket.hpp"

bool isAllDigits(char const *str)
{
    while (*str) {
        if (!std::isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main(int ac, char const **av)
{
    if (ac != 2) {
        std::cerr << "USAGE: ./r-type_server port" << std::endl;
        return 84;
    }
    if (!isAllDigits(av[1])) {
        std::cerr << "Error: Port must be a digit" << std::endl;
        return 84;
    }
    Socket socket(av[1]);
    socket.createSocket();
    socket.run();
    return 0;
}
