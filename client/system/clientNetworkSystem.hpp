/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** networkSystem
*/

#ifndef NETWORKSYSTEM_HPP_
#define NETWORKSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../network/tcpClient/tcpClient.hpp"
#include "../../network/udp/udp.hpp"
#include "../../ecs/event/keyPressed.hpp"
#include "../../ecs/event/shoot.hpp"

class NetworkSystem : public ISystem {
    public:
        NetworkSystem() = delete;
        NetworkSystem(registry &reg, Udp &udpClient, TCPClient &tcpClient): _reg(reg), _udpClient(udpClient), _tcpClient(tcpClient) {};
        ~NetworkSystem() = default;
        void operator()() override {
            _udpClient.mtx.lock();
            while (_udpClient._queue.size() > 0) {
                _udpClient.updateSparseArray(true);
            }
            _udpClient.mtx.unlock();
            if(_reg._eventManager.checkEvent<keyPressed>()) {
                auto &velocity = _reg.getComponent<Velocity>();
                auto &position = _reg.getComponent<Position>();
                if (position[_reg._player]) {
                    _udpClient.sendClientToServer(DATA_PACKET, position[_reg._player].value(), _reg._player);
                    _udpClient.sendClientToServer(DATA_PACKET, velocity[_reg._player].value(), _reg._player);
                }
            }
            if(_reg._eventManager.checkEvent<shoot>()) {
                auto &tmp = _reg._eventManager.getEvent<shoot>();
                for_each(tmp.begin(), tmp.end(), [this](auto &tmp) {
                    _udpClient.sendClientToServer(*tmp, tmp->entity_id);
                });
            }
        };
    private:
        registry &_reg;
        Udp &_udpClient;
        TCPClient &_tcpClient;
};

#endif /* !NETWORKSYSTEM_HPP_ */
