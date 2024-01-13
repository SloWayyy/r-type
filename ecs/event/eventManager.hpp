/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** event
*/

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <memory>
#include "IEvent.hpp"

class EventManager {
    public:
        EventManager() = default;
        ~EventManager() = default;

        template<typename T, typename ...Arg>
        void addEvent(Arg ...arg) {
           _events[std::type_index(typeid(T))].push_back(std::make_unique<T>(arg...));
        }

        template<typename T>
        void addEvent(T event) {
            _events[std::type_index(typeid(T))].push_back(std::make_unique<T>(event));
        }

        template<typename T>
        void addEvent() {
           _events[std::type_index(typeid(T))];
        }

        template<typename T>
        std::vector<std::unique_ptr<T>> &getEvent() {
            try {
                _events.at(std::type_index(typeid(T)));
            } catch (const std::exception &e) {
                this->addEvent<T>();
            }
            auto &tmp = _events.at(std::type_index(typeid(T)));
            return reinterpret_cast<std::vector<std::unique_ptr<T>> &>(tmp);
        }

        void clearEvents() {
            for_each(_events.begin(), _events.end(), [](auto &pair) {
                pair.second.clear();
            });
        }

        template<typename T>
        bool checkEvent() {
            try {
               _events.at(std::type_index(typeid(T)));
            } catch (const std::exception &e) {
                _events[std::type_index(typeid(T))];
                return false;
            }
            return !_events.at(std::type_index(typeid(T))).empty();
        }

    private:
        std::unordered_map<std::type_index, std::vector<std::unique_ptr<IEvent>>> _events;
};

#endif /* !EVENT_HPP_ */
