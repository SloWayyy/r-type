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

bool isDigit(const std::string &port)
{
    return std::all_of(port.begin(), port.end(), ::isdigit);
}

int main(int ac, char const **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_server port ip" << std::endl;
        return FAILURE;
    }
    std::string portStr = av[1];
    if (!isDigit(portStr) || std::stoi(portStr) <= 0) {
        std::cerr << "Error: Port must be a digit" << std::endl;
        return FAILURE;
    }
    Server server(std::atoi(av[1]), 4242, av[2]);
    // server.run();
    return SUCCESS;
}
