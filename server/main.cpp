/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include <iostream>
#include <asio.hpp>

#include "tcp/tcp.hpp"
#include "udp/udp.hpp"
#include "server.hpp"

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
    Server server(std::atoi(av[1]));
    return 0;
}
