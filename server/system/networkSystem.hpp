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
        NetworkSystem(registry &reg, UDPServer &udpServer, TCPServer &tcpServer): _reg(reg), _tcpServer(tcpServer), _udpServer(udpServer) {};
        ~NetworkSystem() = default;
        void operator()() override {
            auto &position = _reg.getComponent<Position>();

            _udpServer.mtxSendPacket.lock();
            // verif si les clients ont tous repondu au packet sionn on renvoi
            _udpServer.mtxSendPacket.unlock();
            _udpServer.mtxQueue.lock();
            while (_udpServer._queue.size() > 0) {
                std::cout << "Queue, il y a des choses a traiter" << std::endl;
                // traiter les packets
                // ajouter dans la queueSendPacket aussi
                _udpServer.saveData();
            }
            _udpServer.mtxQueue.unlock();
        };
    private:
        registry &_reg;
        UDPServer &_udpServer;
        TCPServer &_tcpServer;
};

#endif /* !NETWORKSYSTEM_HPP_ */
