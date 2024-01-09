/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** networkSystem
*/

#ifndef NETWORKSYSTEM_HPP_
#define NETWORKSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../event/bullet.hpp"
#include "../../network/tcpServer/tcpServer.hpp"
#include "../../network/udp/udp.hpp"
#include <chrono>
#include <iostream>

class NetworkSystem : public ISystem {
    public:
        NetworkSystem() = delete;
        NetworkSystem(registry &reg, Udp &udpServer, TCPServer &tcpServer): _reg(reg), _udpServer(udpServer), _tcpServer(tcpServer), _start(std::chrono::system_clock::now()) {};
        ~NetworkSystem() = default;

        void operator()() override {
            std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - _start;
            if (elapsed_seconds.count() >= 0.2) {
                _udpServer.mtxSendPacket.lock();
                for (auto &queue : _udpServer._queueSendPacket) {
                    std::cout << "send packet to client" << std::endl;
                    _udpServer.sendServerToAClient(queue.second, queue.first);
                }
                _udpServer.mtxSendPacket.unlock();
                _start = std::chrono::system_clock::now();
            }
            // verif si les clients ont tous repondu au packet sionn on renvoi
            // si le packet est ancien : on recup le packet, meme le composant apres
            // ATTENTION VERFIER AUSSI LE REMOTE ENDPOINT COMME DANS HANDLERECEIVE SERVER
            _udpServer.mtxQueue.lock();
            while (_udpServer._queue.size() > 0) {
                _udpServer.updateSparseArray(false);
            }
            _udpServer.mtxQueue.unlock();

            auto &sprite = _reg.getComponent<Sprite>();
            auto &position = _reg.getComponent<Position>();
            auto &size = _reg.getComponent<Size>();
            auto &velocity = _reg.getComponent<Velocity>();
            auto &collision = _reg.getComponent<CollisionGroup>();
            auto &hitbox = _reg.getComponent<HitBox>();

            if(_reg._eventManager.checkEvent<bullet>()) {
                for (auto &tmp : _reg._eventManager.getEvent<bullet>()) {
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, sprite[tmp->entity_id].value(), tmp->entity_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, position[tmp->entity_id].value(), tmp->entity_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, size[tmp->entity_id].value(), tmp->entity_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, velocity[tmp->entity_id].value(), tmp->entity_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, collision[tmp->entity_id].value(), tmp->entity_id);
                    _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hitbox[tmp->entity_id].value(), tmp->entity_id);
                }
            }
        };

    private:
        registry &_reg;
        Udp &_udpServer;
        TCPServer &_tcpServer;
        std::chrono::time_point<std::chrono::system_clock> _start;
};

#endif /* !NETWORKSYSTEM_HPP_ */
