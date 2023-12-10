/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <asio.hpp>
#include <iostream>
#include <functional>
#include <array>
#include <memory>
#include "tcp/tcp.hpp"

int main(int ac, char **av)
{
    if (ac != 2) {
        std::cerr << "USAGE: ./r-type_client port" << std::endl;
        return 84;
    }
    try {
        TCPClient client(std::atoi(av[1]));
        while (1) {
            // start game sfml interface
        }
    }
    catch (std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
