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
#include "../../ecs/event/collision.hpp"
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
        };

    private:
        registry &_reg;
        Udp &_udpServer;
        TCPServer &_tcpServer;
        std::chrono::time_point<std::chrono::system_clock> _start;
};

#endif /* !NETWORKSYSTEM_HPP_ */
