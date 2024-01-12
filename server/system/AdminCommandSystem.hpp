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
#define IMMORTAL "/god"

#include "../../ecs/system/ISystem.hpp"
#include "../../ecs/registry/registry.hpp"
#include "../../network/tcpServer/tcpServer.hpp"
#include "../../network/udp/udp.hpp"


class adminCommandSystem : public ISystem {
    public:
        adminCommandSystem() = delete;
        adminCommandSystem(registry &reg, TCPServer &tcpServer, Udp &udpServer): _reg(reg), _tcpServer(tcpServer), _udpServer(udpServer) {};
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
            std::size_t id = _tcpServer.getClientByIndex(std::stoi(command[1]));

            if (command.size() != 2)
                return;
            if (id == PLAYER_NOT_FOUND)
                return;
            std::cout << "healing" << std::endl;
            auto &health = _reg.getComponent<Health>()[std::stoi(command[1])];
            health.value().health = 3;
            auto &pos = _reg.getComponent<Position>()[std::stoi(command[1])];
            pos.value().x = 200;
            pos.value().y = 100;
            _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, health.value(), std::stoi(command[1]));
            _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, pos.value(), std::stoi(command[1]));
            std::cout << "send to all" << std::endl;
        }
        void dead(std::vector<std::string> command) {
            std::size_t id = _tcpServer.getClientByIndex(std::stoi(command[1]));

            if (command.size() != 2)
                return;
            if (id == PLAYER_NOT_FOUND)
                return;
            std::cout << "dead" << std::endl;
            auto &hp = _reg.getComponent<Health>()[std::stoi(command[1])];
            hp.value().health = 0;
            _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hp.value(), std::stoi(command[1]));
            
        }
        void god(std::vector<std::string> command) {
            std::size_t id = _tcpServer.getClientByIndex(std::stoi(command[1]));

            if (command.size() != 2)
                return;
            if (id == PLAYER_NOT_FOUND)
                return;
            std::cout << "god" << std::endl;
            auto &hp = _reg.getComponent<Health>()[std::stoi(command[1])];
            hp.value().health = 1000;
            _udpServer.sendToAll(DATA_PACKET, DATA_PACKET, hp.value(), std::stoi(command[1]));
            
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
        Udp &_udpServer;
        const std::unordered_map<std::string, std::function<void(std::vector<std::string>)>> _adminCommand = {
            {HEALING, [this](std::vector<std::string> command) {healing(command);}},
            {DEAD, [this](std::vector<std::string> command) {dead(command);}},
            {IMMORTAL, [this](std::vector<std::string> command) {god(command);}}
        };
};

#endif /* !ANIMSYSTEM_HPP_ */
