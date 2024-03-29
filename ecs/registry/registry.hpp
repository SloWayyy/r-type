/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** registry
*/

#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_

    #include "../component/component.cpp"
    #include "../system/ISystem.hpp"
    #include "sparse_array/sparse_array.hpp"
    #include "../event/eventManager.hpp"
    #include <any>
    #include <functional>
    #include <map>
    #include <memory>
    #include <optional>
    #include <queue>
    #include <typeindex>
    #include <typeinfo>
    #include <unordered_map>

using Entity = uint32_t;

class registry {
    public:
        registry() = default;
        ~registry() = default;
        template <typename T>
        void addComponent(Sparse_array<T> component)
        {
            std::any tmp = component;
            std::type_index type = std::type_index(tmp.type());

            if (_components.find(type) == _components.end()) {
                _eraseFunction.push_back([](registry &reg, int const &entity) {
                    auto &tmp = reg.getComponent<T>();
                    tmp[entity] = std::optional<T>();
                });

                _addFunction.push_back([](registry &reg, int const &entity) {
                    auto &tmp = reg.getComponent<T>();
                    tmp.push_back();
                });

                _addPacketFunction.push_back([](registry &reg, size_t const &entity, char *packet) {
                    auto &tmp = reg.getComponent<T>();
                    tmp.insert_packet(entity, packet);
                });

                _components.insert(std::pair{type, std::move(tmp)});
            } else {
                std::cerr << "Component already exists" << std::endl;
            }
        };
        template <typename T>
        void addComponent()
        {
            std::any component = Sparse_array<T>();
            std::type_index type = std::type_index(component.type());

            if (_components.find(type) == _components.end()) {

                _typeIndex.push_back(typeid(T));

                _eraseFunction.push_back([](registry &reg, int const &entity) {
                    auto &tmp = reg.getComponent<T>();
                    tmp[entity] = std::optional<T>();
                });

                _addFunction.push_back([](registry &reg, int const &) {
                    auto &tmp = reg.getComponent<T>();
                    tmp.push_back();
                });

                _addPacketFunction.push_back([](registry &reg, int const &entity, char *packet) {
                    auto &tmp = reg.getComponent<T>();
                    tmp.insert_packet(entity, packet);
                });
                _components.insert(std::pair{type, std::move(component)});
            }

            else {
                std::cerr << "Component already exists" << std::endl;
            }
        };
        template <typename T, typename... Params>
        void addAllComponents()
        {
            addComponent<T>();
            if constexpr (sizeof...(Params) > 0) {
                addAllComponents<Params...>();
            }
        };

        template <typename T>
        Sparse_array<T> &getComponent()
        {
            std::type_index type = std::type_index(typeid(Sparse_array<T>));
            auto &t = std::any_cast<Sparse_array<T> &>(_components.at(type));
            return t;
        };

        void removeEntity(Entity const entity)
        {
            if (entity >= DEFAULT_SIZE)
                return;
            for (auto &func : _eraseFunction) {
                func(*this, entity);
            }
            if (std::find(_entity_graveyard.begin(), _entity_graveyard.end(), entity) != _entity_graveyard.end())
                return;
            _entity_graveyard.push_back(entity);
        };

        Entity addEntity()
        {
            if (!_entity_graveyard.empty()) {
                Entity tmp = _entity_graveyard.back();
                _entity_graveyard.erase(_entity_graveyard.end() - 1);
                return tmp;
            }
            if (_entity_count >= DEFAULT_SIZE)
                throw std::runtime_error("Entity limit reached");
            _entity_count++;
            return _entity_count - 1;
        };

        template <class Class, typename... Params>
        void add_system(Params &&...params)
        {
            _system.push_back(std::make_unique<Class>(*this, std::forward<Params>(params)...));
        };

        void run_system()
        {
            for (auto &system : _system) {
                system->operator()();
            }
            _eventManager.clearEvents();
        };

        void registerPacket(size_t type, size_t entity, char *packet)
        {
            _addPacketFunction.at(type)(*this, entity, packet);
        };

        void setPlayerEntity(int player)
        {
            _player = player;
        };

        uint8_t getPlayerEntity()
        {
            return _player;
        };

        template <typename T>
        std::optional<uint32_t> findTypeIndex(T component)
        {
            for (uint32_t type_index = 0; type_index < _typeIndex.size(); type_index++) {
                if (_typeIndex[type_index] == std::type_index(typeid(component)))
                    return type_index;
            }
            return std::nullopt;
        };

        EventManager _eventManager;
        std::vector<std::type_index> _typeIndex;
        std::unordered_map<std::type_index, std::any> _components;
        uint32_t _player = 0;
        uint32_t _entity_count = 0;

    private:
        std::vector<std::unique_ptr<ISystem>> _system;
        std::vector<Entity> _entity_graveyard;
        std::vector<std::function<void(registry &, Entity const &)>> _eraseFunction;
        std::vector<std::function<void(registry &, Entity const &)>> _addFunction;
        std::vector<std::function<void(registry &, size_t const &, char *)>> _addPacketFunction;
        std::map<std::string, Entity> _linker;
};

#endif /* !REGISTRY_HPP_ */
