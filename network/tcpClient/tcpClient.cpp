/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** tcp.cpp
*/

#include "tcpClient.hpp"

TCPClient::TCPClient(std::size_t port, std::string ip, registry &reg)
    : _port(port),
    _ioContext(),
    _socket(_ioContext),
    _resolver(_ioContext),
    _endpoint(asio::ip::make_address(ip), this->_port),
    _ip(ip),
    buffer(),
    reg(reg)
{
    this->createClient();
    this->_thread = std::thread(&TCPClient::run, this);
    this->startAsyncOperations();
}

TCPClient::~TCPClient()
{
    try {
        this->_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error joining thread: " << e.what() << std::endl;
    }
}

std::vector<std::string> TCPClient::getServerMessages()
{
    return this->_ServerMessages;
}

void TCPClient::createClient()
{
    try {
        asio::connect(this->_socket, this->_resolver.resolve(_ip, std::to_string(this->_port)));
        std::cout << "Connected to " << this->_endpoint << " successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

void TCPClient::handleSend(const asio::error_code &error, std::size_t)
{
    if (!error) {
        std::cout << "Message sent" << std::endl;
    } else {
        std::cerr << "Error sending message: " << error.message() << std::endl;
    }
}

void TCPClient::run()
{
    std::cout << "Client is running on port " << this->_port << std::endl;
    this->_ioContext.run();
}

void TCPClient::sendToServer()
{
    std::string message("RECU");
    asio::async_write(this->_socket, asio::buffer(message + "\n"), std::bind(&TCPClient::handleSend, this, std::placeholders::_1, std::placeholders::_2));
}

void TCPClient::handleReceive()
{
    asio::async_read_until(this->_socket, this->buffer, "\n", [this](const asio::error_code &error, std::size_t) {
        if (!error) {
            std::istream input_stream(&this->buffer);
            std::string data;
            std::getline(input_stream, data);
            if (data.find("RFC") != std::string::npos) {
                // std::cout << "RFC FROM SERVER RECEIVER: " + data << std::endl;
                std::cout << data << std::endl;
                this->_ServerMessages.push_back(data); // stocker toutes les commades reçues du serveur et ensuite recuperer ce vector en faisant un getter pour le passer au game
            }
            handleReceive();
        } else {
            std::cerr << "Server is disconected." << std::endl;
            // return;
            exit(0);
        }
    });
}

void TCPClient::startAsyncOperations()
{
    this->handleReceive();
}
