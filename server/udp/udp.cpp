/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

UDPSERVER::UDPSERVER(std::size_t port)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    std::cout << "Constructing UDPSERVER..." << port << std::endl;
    start_receive();
    _io_context.run();
    std::cout << "AFTER RUN" << port << std::endl;
}

void UDPSERVER::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UDPSERVER::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void UDPSERVER::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::string received_message(recv_buffer_.data(), bytes_transferred);
        std::cout << "Message reçu : " << received_message << std::endl;

        // pour avoir l'adresse du client et lui renvoyer un message
        // std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
        // socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
        //     std::bind(&UDPSERVER::handle_send, this, message,
        //     std::placeholders::_1,
        //     std::placeholders::_2));
        // std::cout << "Message recu" << message << std::endl;
        start_receive();
    }
}

void UDPSERVER::handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred)
{
}
