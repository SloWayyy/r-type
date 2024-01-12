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
#include "../../network/tcpClient/tcpClient.hpp"
#include "../../ecs/event/textEntered.hpp"

class messageSystem : public ISystem {
    public:
        messageSystem() = delete;
        messageSystem(registry &reg, TCPClient &tcpClient): _reg(reg), _tcpClient(tcpClient) {};
        ~messageSystem() = default;
        void operator()() override {
                auto &tmp = _reg._eventManager.getEvent<textEntered>();
                for_each(tmp.begin(), tmp.end(), [this](auto &tmp) {
                    // std::cout << "Message " << tmp->_key << std::endl;
                    if ((tmp->_key == '\r' || tmp->_key == '\n') && _textMode == 0) {
                        _textMode = 1;
                    } else if (_textMode == 1) {
                        _message += tmp->_key;
                        if (tmp->_key == '\r' || tmp->_key == '\n') {
                            if (_message.size() != 11) {
                                _tcpClient.sendToServer(_message);
                            }
                            _message = "(RFC) 210 ";
                            _textMode = 0;
                        }
                    }
                });
        };
    private:
        int _textMode = 0;
        std::string _message = "(RFC) 210 ";
        registry &_reg;
        TCPClient &_tcpClient;
};

#endif /* !ANIMSYSTEM_HPP_ */
