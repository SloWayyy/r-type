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

void registry::add_component(std::any component)
{
    std::type_index type = std::type_index(component.type());
    if (_component_arrays.find(type) == _component_arrays.end())
    {
        _component_arrays.insert({type, std::make_unique<std::any>()});
    }
    else
    {
        std::cerr << "Component already exists" << std::endl;
    }
}

