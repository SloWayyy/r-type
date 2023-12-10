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
            void drawEntity(sf::RenderWindow &window);
            void animeEntity(size_t incrementLeft, size_t maxWidth);

            template <typename T>
            void moveEntity(int x, int y) {
                T op;
                auto &position = reg.getComponent<Position>();

                for (int i = 0; i < position.size(); i++) {
                    if (position[i]) {
                        position[i].value().x = op(position[i].value().x, x);
                        position[i].value().y = op(position[i].value().y, y);
                    }
                }
            }

        private:
            registry &reg;
    };

#endif /* !SYSTEM_HPP_ */
