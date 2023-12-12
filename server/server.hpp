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
    Server(std::size_t PortServerTCP, std::size_t PortServerUDP, std::string ip);
    void run();

private:
    std::shared_ptr<UDPServer> udpServer;
    std::shared_ptr<TCPServer> tcpServer;
    std::size_t _PortServerUDP;
};

#endif /* !SERVER_HPP_ */
