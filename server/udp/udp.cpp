/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

UDPServer::UDPServer(std::size_t port)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    this->_port = socket_.local_endpoint().port();
    start_receive();
}

void UDPServer::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UDPServer::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

void UDPServer::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::string received_message(recv_buffer_.data(), bytes_transferred);
        std::cout << "Message reçu : " << received_message << " de " << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;
        if (_clientsUDP.find(remote_endpoint_.port()) == _clientsUDP.end()) {
            _clientsUDP[remote_endpoint_.port()] = remote_endpoint_;
        }
        // response("Hello, client!");
        start_receive();
    }
}

void UDPServer::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_io_context.run();
}

void UDPServer::response(std::string message)
{
    std::shared_ptr<std::string> message2(new std::string(message));
    socket_.async_send_to(asio::buffer(*message2), remote_endpoint_,
        std::bind(&UDPServer::handle_send, this, message2,
        std::placeholders::_1,
        std::placeholders::_2));
    std::cout << "Message envoyé TO CLIENT" << message << std::endl;
    std::cout << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;

}

void UDPServer::send(std::string message, asio::ip::udp::endpoint endpoint)
{
    try {
        socket_.send_to(asio::buffer(message), endpoint);
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void UDPServer::handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred)
{
    std::cout << "Message envoyé" << std::endl;
}

size_t UDPServer::getPort() const
{
    return this->_port;
}
