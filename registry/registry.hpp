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
#include <map>
#include <functional>
#include "sparse_array/sparse_array.hpp"

class registry
{

public:
    using entity_t = int;

public:
    registry() = default;
    ~registry() = default;
    template <typename T>
    void addComponent(Sparse_array<T> component)
    {
        std::any tmp = component;
        std::type_index type = std::type_index(tmp.type());

        if (_components.find(type) == _components.end())
        {
            _eraseFunction.push_back([](registry &reg, int const &entity) {
                auto &tmp = reg.getComponent<T>();
                tmp[entity] = std::optional<T>();
            });

            _addFunction.push_back([](registry &reg, int const &entity) {
                auto &tmp = reg.getComponent<T>();
                tmp.push_back();
            });
            _components.insert(std::pair{type, std::move(tmp)});
        }
        else
        {
            std::cerr << "Component already exists" << std::endl;
        }
    };
    template <typename T>
    void addComponent()
    {
        std::any component = Sparse_array<T>();

        std::type_index type = std::type_index(component.type());
        if (_components.find(type) == _components.end())
        {
            _eraseFunction.push_back([](registry &reg, int const &entity) {
                auto &tmp = reg.getComponent<T>();
                tmp[entity] = std::optional<T>();
            });

            _addFunction.push_back([](registry &reg, int const &entity) {
                auto &tmp = reg.getComponent<T>();
                tmp.push_back();
            });
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
    void removeEntity(entity_t const &entity)
    {
        if (entity >= _entity_count)
            return;
        if (std::find(_entity_graveyard.begin(), _entity_graveyard.end(), entity) != _entity_graveyard.end())
            return;
        for (auto &func : _eraseFunction)
        {
            func(*this, entity);
        }
        _entity_graveyard.push_back(entity);
    };
    entity_t addEntity()
    {
        if (!_entity_graveyard.empty())
        {
            entity_t tmp = _entity_graveyard.back();
            _entity_graveyard.erase(_entity_graveyard.end() - 1);
            return tmp;
        }

        for (auto &func : _addFunction)
        {
            func(*this, _entity_count);
        }
        _entity_count++;
        return _entity_count - 1;
    };
private:
    std::vector<entity_t> _entity_graveyard;
    std::unordered_map<std::type_index, std::any> _components;
    std::vector<std::function<void(registry &, entity_t const &)>> _eraseFunction;
    std::vector<std::function<void(registry &, entity_t const &)>> _addFunction;
    entity_t _entity_count = 0;
    std::map<std::string, entity_t> _linker;
};
#endif /* !REGISTRY_HPP_ */