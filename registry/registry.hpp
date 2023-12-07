/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** registry
*/

#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <any>
#include "sparse_array/sparse_array.hpp"

class registry
{

public:
    registry();
    ~registry();
    void add_component(std::any component);
    std::unordered_map<std::type_index, std::any> _component_arrays;
private:
    int _entity_count;

};
#endif /* !REGISTRY_HPP_ */