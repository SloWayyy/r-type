/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

Udp_server::Udp_server(asio::io_context& io_context, std::size_t port)
    : socket_(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    start_receive();
    io_context.run();
}