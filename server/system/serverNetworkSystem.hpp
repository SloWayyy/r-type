/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** networkSystem
*/

#ifndef NETWORKSYSTEM_HPP_
#define NETWORKSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/event/destroyEntity.hpp"
#include "../../network/tcpServer/tcpServer.hpp"
#include "../../network/udp/udp.hpp"
#include "../event/bullet.hpp"
#include "../event/spawnedEnnemy.hpp"
#include "../event/health.hpp"
#include "../event/score.hpp"
#include <chrono>
#include <iostream>

class NetworkSystem : public ISystem {
public:
    NetworkSystem() = delete;
    NetworkSystem(registry& reg, Udp& udpServer, TCPServer& tcpServer)
        : _reg(reg)
        , _udpServer(udpServer)
        , _tcpServer(tcpServer)
        , _start(std::chrono::system_clock::now()) {};
    ~NetworkSystem() = default;

    void operator()() override
    {
        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - _start;
        if (elapsed_seconds.count() >= 0.2) {
            _udpServer.mtxSendPacket.lock();
            for (auto& queue : _udpServer._queueSendPacket) {
                _udpServer.sendServerToAClient(queue.second, queue.first);
            }
            _udpServer.mtxSendPacket.unlock();
            _start = std::chrono::system_clock::now();
        }
        _udpServer.mtxQueue.lock();
        while (_udpServer._queue.size() > 0) {
            _udpServer.updateSparseArray(false);
        }
        _udpServer.mtxQueue.unlock();

        auto& sprite = _reg.getComponent<Sprite>();
        auto& position = _reg.getComponent<Position>();
        auto& size = _reg.getComponent<Size>();
        auto& score1 = _reg.getComponent<Score>();
        auto& velocity = _reg.getComponent<Velocity>();
        auto& collision = _reg.getComponent<CollisionGroup>();
        auto& hitbox = _reg.getComponent<HitBox>();
        auto& owner = _reg.getComponent<Owner>();

        if (_reg._eventManager.checkEvent<bullet>()) {
            for (auto& tmp : _reg._eventManager.getEvent<bullet>()) {
                if (position[tmp->shoot_id] && position[tmp->bullet_id] && sprite[tmp->bullet_id] && size[tmp->bullet_id] && velocity[tmp->bullet_id] && collision[tmp->bullet_id] && hitbox[tmp->bullet_id]) {
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, position[tmp->shoot_id].value(), tmp->shoot_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, position[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, sprite[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, size[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, velocity[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, collision[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hitbox[tmp->bullet_id].value(), tmp->bullet_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, owner[tmp->bullet_id].value(), tmp->bullet_id);
                }
            }
        }
        if (_reg._eventManager.checkEvent<spawnedEnnemy>()) {
            for (auto& tmp : _reg._eventManager.getEvent<spawnedEnnemy>()) {
                if (position[tmp->ennemy_id] && sprite[tmp->ennemy_id] && size[tmp->ennemy_id] && velocity[tmp->ennemy_id] && collision[tmp->ennemy_id] && hitbox[tmp->ennemy_id]) {
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, sprite[tmp->ennemy_id].value(), tmp->ennemy_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, position[tmp->ennemy_id].value(), tmp->ennemy_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, size[tmp->ennemy_id].value(), tmp->ennemy_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, velocity[tmp->ennemy_id].value(), tmp->ennemy_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, collision[tmp->ennemy_id].value(), tmp->ennemy_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hitbox[tmp->ennemy_id].value(), tmp->ennemy_id);
                }
            }
        }
        if (_reg._eventManager.checkEvent<destroyEntity>()) {
            for (auto& tmp : _reg._eventManager.getEvent<destroyEntity>()) {
                _udpServer.sendToAll(DESTROY_ENTITY, DESTROY_ENTITY, tmp->entity_id);
            }
        }
        if (_reg._eventManager.checkEvent<health>()) {
            for (auto& tmp : _reg._eventManager.getEvent<health>()) {
                auto &hp = _reg.getComponent<Health>()[tmp->entity_id];
                auto &vel = _reg.getComponent<Velocity>()[tmp->entity_id];
                _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, vel.value(), tmp->entity_id);
                _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hp.value(), tmp->entity_id);
            }
        }
        if (_reg._eventManager.checkEvent<score>()) {
            for (auto& tmp : _reg._eventManager.getEvent<score>()) {
                _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, score1[tmp->entity_id].value(), tmp->entity_id);
            }
        }
    };

private:
    registry& _reg;
    Udp& _udpServer;
    TCPServer& _tcpServer;
    std::chrono::time_point<std::chrono::system_clock> _start;
};

#endif /* !NETWORKSYSTEM_HPP_ */
