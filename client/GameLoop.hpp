/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** GameLoop
*/

#ifndef GAMELOOP_HPP_
    #define GAMELOOP_HPP_

    #include "tcp/tcp.hpp"

class GameLoop {
    public:
        GameLoop(std::size_t port);
        ~GameLoop();
        void run();
    protected:
    private:
        TCPClient _TCPclient;
};

#endif /* !GAMELOOP_HPP_ */