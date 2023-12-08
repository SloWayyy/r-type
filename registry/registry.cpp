/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** registry
*/

#include "registry.hpp"
#include <iostream>

registry::registry()
{
}

registry::~registry()
{
}

void registry::addComponent(std::any component)
{
    std::type_index type = std::type_index(component.type());
    if (_components.find(type) == _components.end())
    {
        _components.insert(std::pair{type, std::move(component)});
    }
    else
    {
        std::cerr << "Component already exists" << std::endl;
    }
}
