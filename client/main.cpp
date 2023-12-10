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
    if (ac != 2) {
        std::cerr << "USAGE: ./r-type_client port" << std::endl;
        return 84;
    }
    try {
        GameLoop gameLoop(std::stoi(av[1]));
        gameLoop.run();
    }
    catch (std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
