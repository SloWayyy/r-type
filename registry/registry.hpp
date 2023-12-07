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
private:
    int _entity_count;
    std::unordered_map<std::type_index, std::unique_ptr<std::any>> _component_arrays;

};
#endif /* !REGISTRY_HPP_ */