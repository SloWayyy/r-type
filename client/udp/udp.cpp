/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

UDPClient::UDPClient(std::size_t port)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), _port(port)
{
    start_receive();
}

void UDPClient::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UDPClient::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void UDPClient::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::string received_message(recv_buffer_.data(), bytes_transferred);
        std::cout << "Message reçu : " << received_message << std::endl;
        start_receive();
    }
}

void UDPClient::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    // this->_io_context.run();
}

void UDPClient::send(std::string message, asio::ip::udp::endpoint endpoint)
{
    socket_.send_to(asio::buffer(message), endpoint);
}

void UDPClient::handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred)
{
    std::cout << "Message envoyé" << std::endl;
}
