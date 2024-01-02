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

class messageSystem : public ISystem {
    public:
        messageSystem() = delete;
        messageSystem(registry &reg, TCPClient &tcpClient): _reg(reg), _tcpClient(tcpClient) {};
        ~messageSystem() = default;
        void operator()() override {
            if (_reg._events.find(Event_t::TEXT_ENTERED) == _reg._events.end())
                return;
            try {
                auto &tmp = _reg._events.at(Event_t::TEXT_ENTERED);
                if ((tmp == '\r' || tmp == '\n') && _textMode == 0) {
                    _textMode = 1;
                } else if (_textMode == 1) {
                    _message += tmp;
                    if (tmp == '\r' || tmp == '\n') {
                        if (_message.size() != 11) {
                            _tcpClient.sendToServer(_message);
                        }
                        _message = "(RFC) 210 ";
                        _textMode = 0;
                    }
                }
                _reg._events.erase(Event_t::TEXT_ENTERED);
            } catch (const std::exception &e) {
                std::cerr << e.what() << '\n';
            }
        };
    private:
        int _textMode = 0;
        std::string _message = "(RFC) 210 ";
        registry &_reg;
        TCPClient &_tcpClient;
};

#endif /* !ANIMSYSTEM_HPP_ */
