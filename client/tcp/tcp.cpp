/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcp.hpp"

TCPClient::TCPClient(std::size_t port) : _port(port), _ioContext(), _socket(_ioContext), _resolver(_ioContext), _endpoint(asio::ip::make_address("127.0.0.1"), this->_port), buffer()
{
    this->createClient();
    while (1) {
        // this->readFromServer();
        this->handleReceive();
        this->sendToServer();
    }
}

void TCPClient::createClient()
{
    asio::connect(this->_socket, this->_resolver.resolve("127.0.0.1", std::to_string(this->_port)));
    std::cout << "Connect to " << this->_endpoint << " successfully!\n";
}

void TCPClient::handle_send(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Message envoyé" << std::endl;
    }
}

void TCPClient::sendToServer()
{
    std::string message;
    std::getline(std::cin, message);
    this->_socket.async_send(asio::buffer(message + "\n"), std::bind(&TCPClient::handle_send, this, std::placeholders::_1, std::placeholders::_2));
}

void TCPClient::readFromServer()
{
    // asio::async_read_until(this->_socket, this->buffer, "\n", [this](const asio::error_code &error, std::size_t bytes_transferred) {
    //     if (!error) {
    //         std::cout << "Message reçu" << std::endl;
    //         std::istream input_stream(&this->buffer);
    //         std::string data;
    //         std::getline(input_stream, data);
    //         std::cout << "Message reçu : " << data << std::endl;
    //     } else {
    //         std::cerr << "Error reading from server: " << error.message() << std::endl;
    //     }
    // });
}

void TCPClient::handleReceive()
{
    asio::async_read_until(this->_socket, this->buffer, "\n", [this](const asio::error_code &error, std::size_t bytes_transferred) {
        if (!error) {
            std::istream input_stream(&this->buffer);
            std::string data;
            std::getline(input_stream, data);
            std::cout << "Message reçu : " << data << std::endl;
            handleReceive();
        } else {
            std::cerr << "Error reading from server: " << error.message() << std::endl;
        }
    });
}
