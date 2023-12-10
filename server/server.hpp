/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** server
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#include "tcp/tcp.hpp"
#include "udp/udp.hpp"

static constexpr int SUCCESS = 0;
static constexpr int FAILURE = 84;

class Server {
public:
    Server(std::size_t Port);
    void run();

private:
    std::shared_ptr<TCPServer> tcpServer;
    std::size_t _PortServerUDP = 5558;
    // std::shared_ptr<UDPSERVER> udpServer;
    // asio::io_context ioContext;
};

#endif /* !SERVER_HPP_ */
