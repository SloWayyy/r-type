/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** serverScoreSystem
*/

#ifndef SERVERSCORESYSTEM_HPP_
    #define SERVERSCORESYSTEM_HPP_

#include "../../ecs/event/collision.hpp"
#include "../../ecs/event/destroyEntity.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../ecs/system/ISystem.hpp"
#include "../../network/udp/udp.hpp"
#include "../../server/event/bullet.hpp"

class ServerScoreSystem : public ISystem {
public:
    ServerScoreSystem() = delete;
    ServerScoreSystem(registry& reg)
       : _reg(reg) {};
    ~ServerScoreSystem() = default;
    void operator()() override
    {
        auto& score1 = _reg.getComponent<Score>();
        auto& owner = _reg.getComponent<Owner>();
        if (_reg._eventManager.checkEvent<collision>()) {
            for (auto& tmp : _reg._eventManager.getEvent<collision>()) {
                if (owner[tmp->_id1]) {
                    score1[owner[tmp->_id1].value().i].value().s += 1;
                    _reg._eventManager.addEvent<score>(owner[tmp->_id1].value().i);
                    std::cout << "score of " << owner[tmp->_id1].value().i << " is now " << score1[owner[tmp->_id1].value().i].value().s << std::endl;
                }
                if (owner[tmp->_id2]) {
                    score1[owner[tmp->_id2].value().i].value().s += 1;
                    _reg._eventManager.addEvent<score>(owner[tmp->_id2].value().i);
                    std::cout << "score of " << owner[tmp->_id2].value().i << " is now " << score1[owner[tmp->_id2].value().i].value().s << std::endl;
                }
            }
        }
    };

private:
    registry& _reg;
};

#endif /* !SERVERSCORESYSTEM_HPP_ */
