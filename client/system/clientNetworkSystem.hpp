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

class NetworkSystem : public ISystem {
    public:
        NetworkSystem() = delete;
        NetworkSystem(registry &reg, Udp &udpClient, TCPClient &tcpClient): _reg(reg), _udpClient(udpClient), _tcpClient(tcpClient) {};
        ~NetworkSystem() = default;
        void operator()() override {
            while (_udpClient._queue.size() > 0) {
                std::cout << "Queue, il y a des choses a traiter" << std::endl;
                auto packet = _udpClient._queue.front();
                _udpClient.mtx.lock();
                _udpClient.updateSparseArray(true);
                _udpClient._queue.erase(_udpClient._queue.begin());
                _udpClient.mtx.unlock();
                // Packet header = packet.first;
                std::vector<uint8_t> component = packet.second;
            }
            if (_reg._events.find(Event_t::KEY_PRESSED) == _reg._events.end())
                return;
            else {
                int teest[] = {0, 1};
                std::cout <<" player ==== " << teest[_reg._player] << std::endl;
                auto &velocity = _reg.getComponent<Velocity>();
                auto &position = _reg.getComponent<Position>();
                _udpClient.sendClientToServer(DATA_PACKET, position[_reg._player].value(), _reg._player);
                _udpClient.sendClientToServer(DATA_PACKET, velocity[_reg._player].value(), _reg._player);
                _reg._events.erase(Event_t::KEY_PRESSED);
            }
        };
    private:
        registry &_reg;
        Udp &_udpClient;
        TCPClient &_tcpClient;
};

#endif /* !NETWORKSYSTEM_HPP_ */
