/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** serverEventLoaderSystem
*/

#ifndef SERVEREVENTLOADERSYSTEM_HPP_
    #define SERVEREVENTLOADERSYSTEM_HPP_

    #include "../../ecs/system/ISystem.hpp"
    #include "../../network/tcpServer/tcpServer.hpp"
    #include "../../network/udp/udp.hpp"

class ServerEventLoaderSystem : public ISystem {
    public:
        ServerEventLoaderSystem() = delete;
        ServerEventLoaderSystem(registry &reg, Udp &udpServer): _reg(reg), _udpServer(udpServer), _start(std::chrono::system_clock::now()) {};
        ~ServerEventLoaderSystem() = default;

        void operator()() override {
            _udpServer._eventmtx.lock();
            for (auto &queue : _udpServer._eventQueue) {
                _reg._eventManager.addEvent(std::any_cast<shoot>(queue));
            }
            _udpServer._eventQueue.clear();
            _udpServer._eventmtx.unlock();
        };

    private:
        registry &_reg;
        Udp &_udpServer;
        std::chrono::time_point<std::chrono::system_clock> _start;
};

#endif /* !SERVEREVENTLOADERSYSTEM_HPP_ */
