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
                    std::cout << "RE SENDING PACKET" << std::endl;
                    // Packet queryPacket;
                    // std::memcpy(&queryPacket, queue.second.data(), sizeof(Packet));
                    // queryPacket.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    // queryPacket.packet_type = REPEAT_PACKET;
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
        };

    private:
        registry &_reg;
        Udp &_udpServer;
        TCPServer &_tcpServer;
        std::chrono::time_point<std::chrono::system_clock> _start;
};

#endif /* !NETWORKSYSTEM_HPP_ */
