/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** adminCommandSystem
*/

#ifndef ADMINCOMMANDSYSTEM_HPP_
#define ADMINCOMMANDSYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../network/tcpServer/tcpServer.hpp"
#include <regex>

class adminCommandSystem : public ISystem {
    public:
        adminCommandSystem() = delete;
        adminCommandSystem(registry &reg, TCPServer &tcpServer): _reg(reg), _tcpServer(tcpServer) {};
        ~adminCommandSystem() = default;
        void operator()() override {
            _tcpServer._mtxQueueAdminCommand.lock();
            while (_tcpServer._queueAdminCommand.size() > 0) {
                auto &tmp = _tcpServer._queueAdminCommand.back();
                // first == port(tcp), second == command(msg)
                std::cout << "admin command: " << tmp.second << std::endl;
                _tcpServer._queueAdminCommand.pop_back();
            }
            _tcpServer._mtxQueueAdminCommand.unlock();
        };
    private:
        registry &_reg;
        TCPServer &_tcpServer;
};

#endif /* !ANIMSYSTEM_HPP_ */
