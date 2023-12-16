/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include "GameLoop.hpp"

int main(int ac, char **av)
{
    if (ac != 3) {
        std::cerr << "USAGE: ./r-type_client port ip" << std::endl;
        return 84;
    }
    try {
        GameLoop gameLoop(std::stoi(av[1]), 4243, av[2]);
        // gameLoop.run();
    }
    catch (std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
