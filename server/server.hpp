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

class Server {
public:
    Server(std::size_t Port);
    void run();

private:
    std::shared_ptr<Tcp_server> tcpServer;
    std::shared_ptr<Udp_server> udpServer;
    asio::io_context ioContext;
};

#endif /* !SERVER_HPP_ */
