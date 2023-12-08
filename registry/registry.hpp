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
    void addComponent(std::any component);
    template <typename T>
    void addComponent()
    {
        std::any component = Sparse_array<T>();

        std::type_index type = std::type_index(component.type());
        if (_components.find(type) == _components.end())
        {
            _components.insert(std::pair{type, std::move(component)});
        }
        else
        {
            std::cerr << "Component already exists" << std::endl;
        }
    };
    template <typename T>
    Sparse_array<T> &getComponent()
    {
        std::type_index type = std::type_index(typeid(Sparse_array<T>));
        auto &t = std::any_cast<Sparse_array<T> &>(_components[type]);
        return t;
    };

private:
    std::unordered_map<std::type_index, std::any> _components;
    int _entity_count;
};
#endif /* !REGISTRY_HPP_ */