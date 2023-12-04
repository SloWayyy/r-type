/*
** EPITECH PROJECT, 2023
** Rtype server
** File description:
** main.cpp
*/

#include <iostream>


int main(int ac, char **av)
{
    (void) av;
    if (ac != 2) {
        std::cerr << "USAGE: ./r-type_server port" << std::endl;
        return 84;
    }
    return 0;
}
