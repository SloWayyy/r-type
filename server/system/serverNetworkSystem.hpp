/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** networkSystem
*/

#ifndef NETWORKSYSTEM_HPP_
#define NETWORKSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../network/tcpServer/tcpServer.hpp"
#include "../../network/udp/udp.hpp"

class NetworkSystem : public ISystem {
    public:
        NetworkSystem() = delete;
        NetworkSystem(registry &reg, Udp &udpServer, TCPServer &tcpServer): _reg(reg), _udpServer(udpServer), _tcpServer(tcpServer) {};
        ~NetworkSystem() = default;

        void operator()() override {
            _udpServer.mtxSendPacket.lock();
            // verif si les clients ont tous repondu au packet sionn on renvoi
            _udpServer.mtxSendPacket.unlock();
            _udpServer.mtxQueue.lock();
            while (_udpServer._queue.size() > 0) {
            std::cout << "Queue size: " << _udpServer._queue.size() << std::endl;
                _udpServer.updateSparseArray(false);
            }
            _udpServer.mtxQueue.unlock();
        };

    private:
        registry &_reg;
        Udp &_udpServer;
        TCPServer &_tcpServer;
};

#endif /* !NETWORKSYSTEM_HPP_ */
