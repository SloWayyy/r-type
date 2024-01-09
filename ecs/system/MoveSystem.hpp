/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** system
*/

#define AVERAGE_SIZE_SPRITE 32

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include "../registry/registry.hpp"
    #include "../event/collision.hpp"

class MoveSystem : public ISystem {
    public:
        MoveSystem() = delete;
        MoveSystem(registry &reg): _reg(reg) {};
        ~MoveSystem() = default;

        bool isColliding(Sparse_array<Position> &position, Sparse_array<Velocity> &velocity, Sparse_array<HitBox> &hitbox) {
            // static int toto = 0;
            // toto++;
            // // std::cout << "position size: " << position.size() << " velocity size: " << velocity.size() << " hitbox size: " << hitbox.size() << std::endl;
            // std::cout << "toto: " << toto << std::endl;
            // if (toto > 150) {
            //         std::cout <<"CAAAAA:"<< position[0].value().x  << " velo: "<< velocity[0].value().x_speed << " hitbox " << hitbox[0].value().w << std::endl;
            //         std::cout <<"CAAAAA:"<< position[4].value().x  << " velo: "<< velocity[4].value().x_speed << " hitbox " << hitbox[4].value().w << std::endl;
            //         std::cout <<"CAAAAA:"<< position[5].value().x  << " velo: "<< velocity[5].value().x_speed << " hitbox " << hitbox[5].value().w << std::endl;
            // }
            for (long unsigned int i = 0; i < DEFAULT_SIZE; i++) {
                if (position[i] && velocity[i] && hitbox[i]) {
                    for (long unsigned int j = 0; j < DEFAULT_SIZE; j++) {
                        if (position[j] && velocity[j] && i != j && hitbox[j]) {
                            // std::cout << "player : " << std::endl;
                            // position[0].value().print();
                            // std::cout << "ennemy : "<< std::endl;
                            // position[5].value().print();

                            if (position[i].value().x + velocity[i].value().x_speed < position[j].value().x + hitbox[j].value().w &&
                                position[i].value().x + velocity[i].value().x_speed + hitbox[i].value().h > position[j].value().x &&

                                position[i].value().y + velocity[i].value().y_speed < position[j].value().y + hitbox[j].value().w &&
                                position[i].value().y + velocity[i].value().y_speed + hitbox[i].value().h > position[j].value().y) {

                                _reg._eventManager.addEvent<collision>(i, j);
                                std::cout << "||||||||||||||||||||||--collision--|||||||||||||||||----" << std::endl;
                                velocity[i].value().x_speed = 0;
                                velocity[i].value().y_speed = 0;
                                return true;
                            }
                        }
                    }
                }
            }
            return false;
        }

        void operator()() override {
            auto &position = _reg.getComponent<Position>();
            auto &velocity = _reg.getComponent<Velocity>();
            auto &hitbox = _reg.getComponent<HitBox>();

            isColliding(position, velocity, hitbox);

            for (long unsigned int i = 0; i < position.size(); i++) {
                if (position[i] && velocity[i]) {
                    position[i].value().x += velocity[i].value().x_speed;
                    position[i].value().y += velocity[i].value().y_speed;
                }
            }
        };
    private:
        registry &_reg;
};

#endif /* !SYSTEM_HPP_ */
