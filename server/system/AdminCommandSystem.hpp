/*
** EPITECH PROJECT, 2023
** R-type
** File description:
** adminCommandSystem
*/

#ifndef ADMINCOMMANDSYSTEM_HPP_
#define ADMINCOMMANDSYSTEM_HPP_
#define HEALING "/heal"
#define DEAD "/dead"

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../network/tcpServer/tcpServer.hpp"


class adminCommandSystem : public ISystem {
    public:
        adminCommandSystem() = delete;
        adminCommandSystem(registry &reg, TCPServer &tcpServer): _reg(reg), _tcpServer(tcpServer) {};
        ~adminCommandSystem() = default;
        std::vector<std::string> split(const std::string &str, char delim) {
            std::vector<std::string> result;
            std::stringstream ss(str);
            std::string item;

            while (getline(ss, item, delim)) {
                result.push_back(item);
            }
            return result;
        }
        void healing(std::vector<std::string> command) {
            if (command.size() != 2)
                return;
            std::cout << "healing" << std::endl;
        }
        void dead(std::vector<std::string> command) {
            if (command.size() != 2)
                return;
            std::cout << "dead" << std::endl;
        }
        void operator()() override {
            _tcpServer._mtxQueueAdminCommand.lock();
            while (_tcpServer._queueAdminCommand.size() > 0) {
                auto &tmp = _tcpServer._queueAdminCommand.back();
                std::vector<std::string> result = split(tmp.second, ' ');
                std::cout << "admin command: " << tmp.second << std::endl;
                if (_adminCommand.find(result[0]) != _adminCommand.end()) {
                    _adminCommand.at(result[0])(result);
                }
                _tcpServer._queueAdminCommand.pop_back();
            }
            _tcpServer._mtxQueueAdminCommand.unlock();
        };
    private:
        registry &_reg;
        TCPServer &_tcpServer;
        const std::unordered_map<std::string, std::function<void(std::vector<std::string>)>> _adminCommand = {
            {HEALING, [this](std::vector<std::string> command) {healing(command);}},
            {DEAD, [this](std::vector<std::string> command) {dead(command);}},
        };
};

#endif /* !ANIMSYSTEM_HPP_ */
