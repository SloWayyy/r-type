/*
** EPITECH PROJECT, 2024
** r-type
** File description:
** audioSystem
*/

#ifndef AUDIOSYSTEM_HPP_
    #define AUDIOSYSTEM_HPP_

    #include "../../ecs/system/ISystem.hpp"
    #include "../../network/tcpClient/tcpClient.hpp"
    #include "../../network/udp/udp.hpp"
    #include "../../ecs/event/keyPressed.hpp"
    #include "../../ecs/event/shoot.hpp"

class AudioSystem : public ISystem {
    public:
        AudioSystem() = delete;
        AudioSystem(registry &reg, Udp &udpClient, TCPClient &tcpClient): _reg(reg), _udpClient(udpClient), _tcpClient(tcpClient) {};
        ~AudioSystem() = default;
        void operator()() override {
            
        };
    private:
        registry &_reg;
        Udp &_udpClient;
        TCPClient &_tcpClient;
};

#endif /* !AUDIOSYSTEM_HPP_ */
