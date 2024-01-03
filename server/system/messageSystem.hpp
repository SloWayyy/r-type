/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** messageSystem
*/

#ifndef MESSAGESYSTEM_HPP_
#define MESSAGESYSTEM_HPP_

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../network/tcpServer/tcpServer.hpp"

class messageSystem : public ISystem {
    public:
        messageSystem() = delete;
        messageSystem(registry &reg, TCPServer &tcpServer): _reg(reg), _tcpServer(tcpServer) {};
        ~messageSystem() = default;
        void operator()() override {
            while (_tcpServer._ClientMessages.size() > 0) {
                auto &tmp = _tcpServer._ClientMessages.back();
                if (tmp.size() > 10 && tmp.substr(0, 10) == "(RFC) 210 ") {
                    _tcpServer.sendMessageToAllClients(tmp);
                }
                _tcpServer._ClientMessages.pop_back();
            }
        };
    private:
        int _textMode = 0;
        registry &_reg;
        TCPServer &_tcpServer;
};

#endif /* !ANIMSYSTEM_HPP_ */
