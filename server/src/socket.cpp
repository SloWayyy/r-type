/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "socket.hpp"

Socket::Socket(char const *port) : _port(std::atoi(port)), _endpoint(asio::ip::tcp::v6(), std::atoi(port)), _client(_ioContext), _acceptor(_ioContext, _endpoint), buffer()
{
}

asio::ip::tcp::socket &Socket::getClient()
{
    return this->_client;
}

asio::io_context &Socket::getIoContext()
{
    return this->_ioContext;
}

asio::error_code &Socket::getEc()
{
    return this->_ec;
}

asio::ip::tcp::endpoint &Socket::getEndpoint()
{
    return this->_endpoint;
}

asio::ip::tcp::acceptor &Socket::getAcceptor()
{
    return this->_acceptor;
}

asio::streambuf &Socket::getBuffer()
{
    return this->buffer;
}

void Socket::createSocket()
{
    this->_acceptor.listen();
    std::cout << "Server socket created and bound to port " << this->_port << std::endl;
}

void Socket::handleRead()
{
    std::size_t bytesRead = asio::read_until(this->_client, this->buffer, "\n");
    if (bytesRead > 0) {
        std::istream input(&this->buffer);
        std::string data;
        std::getline(input, data);
        std::cout << "Received data: " << data << std::endl;
        std::string response = "Server response: " + data + "\n";
        asio::write(this->_client, asio::buffer(response));
    } else {
        std::cerr << "Error reading data from the client" << std::endl;
    }
}

void Socket::run()
{
    while (1) {
        this->_acceptor.accept(this->_client);
        std::cout << "Accepted connection from: " << this->_client.remote_endpoint() << std::endl;
        while (1) {
            handleRead();
        }
    }
}
