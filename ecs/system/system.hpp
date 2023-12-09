/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../registry/registry.hpp"

    class System {
        public:
            System(registry &reg) : reg(reg) {};
            ~System() = default;
            void display_drawable();
        private:
            registry &reg;
    };

#endif /* !SYSTEM_HPP_ */
