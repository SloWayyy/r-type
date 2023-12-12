/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop
*/

#ifndef GAMELOOP_HPP_
    #define GAMELOOP_HPP_

    #include "tcp/tcp.hpp"
    #include "udp/udp.hpp"

class GameLoop {
    public:
        GameLoop(std::size_t TCPport, std::size_t UDPport, std::string ip);
        ~GameLoop();
        void run();
    protected:
    private:
        TCPClient _TCPclient;
        UDPClient _UDPclient;
        std::size_t _TCPport;
        std::size_t _UDPport;
};

#endif /* !GAMELOOP_HPP_ */