/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** networkSystem
*/

#ifndef NETWORKSYSTEM_HPP_
#define NETWORKSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../tcp/tcp.hpp"
#include "../udp/udp.hpp"

class NetworkSystem : public ISystem {
    public:
        NetworkSystem() = delete;
        NetworkSystem(registry &reg, UDPClient &udpClient, TCPClient &tcpClient): _reg(reg), _tcpClient(tcpClient), _udpClient(udpClient) {};
        ~NetworkSystem() = default;
        void operator()() override {
            while (_udpClient._queue.size() > 0) {
                std::cout << "Queue, il y a des choses a traiter" << std::endl;
                auto packet = _udpClient._queue.front();
                _udpClient.mtx.lock();
                _udpClient.saveData();
                _udpClient._queue.erase(_udpClient._queue.begin());
                _udpClient.mtx.unlock();
                Packet header = packet.first;
                std::vector<uint8_t> component = packet.second;
            }
            if (_reg._events.find(Event_t::KEY_PRESSED) == _reg._events.end())
                return;
            else {
                int teest[] = {0, 1};
                std::cout <<" player ==== " << teest[_reg._player] << std::endl;
                 auto &velocity = _reg.getComponent<Velocity>();
                _udpClient.send(velocity[_reg._player].value(), _reg._player, DATA_PACKET);
                _reg._events.erase(Event_t::KEY_PRESSED);
            }
        };
    private:
        registry &_reg;
        UDPClient &_udpClient;
        TCPClient &_tcpClient;
};

#endif /* !NETWORKSYSTEM_HPP_ */
