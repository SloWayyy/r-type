/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcp.hpp"

TCPServer::TCPServer(std::size_t port) : _port(port),  _ioContext(), _endpoint(asio::ip::tcp::v4(), port), _acceptor(_ioContext, _endpoint), buffer()
{
    this->createSocket();
    this->startAccept();
    this->_ioContext.run();
}

void TCPServer::createSocket()
{
    std::cout << "Creating server socket..." << std::endl;
    this->_acceptor.listen();
}

void TCPServer::handleRead(std::shared_ptr<asio::ip::tcp::socket> client)
{
    asio::async_read_until(*client, buffer, "\n", [this, client](const asio::error_code& ec, std::size_t bytesRead) {
        if (!ec) {
            std::istream input(&this->buffer);
            std::string data;
            std::getline(input, data);
            std::cout << "Client: " << client->remote_endpoint().address() << ":" << client->remote_endpoint().port() << " Sent: " << data << std::endl;
            handleRead(client);
        } else {
            std::cerr << "client " << client->remote_endpoint() << " is disconnected."  << std::endl;
            _clientsConnected.erase(std::remove(_clientsConnected.begin(), _clientsConnected.end(), client), _clientsConnected.end());
        }
    });
}

void TCPServer::sendMessageToClients(const std::string &message)
{
    for (const auto &client : _clientsConnected) {
        client->async_send(asio::buffer(message + "\n"), [this, client](const asio::error_code& ec, std::size_t bytesSent) {
            if (!ec) {
                std::cout << "Message sent to client " << client->remote_endpoint() << std::endl;
            } else {
                std::cerr << "Error sending message to client " << client->remote_endpoint() << ": " << ec.message() << std::endl;
            }
        });
    }
}

void TCPServer::handleSend()
{
    std::string messageFromServer;
    std::getline(std::cin, messageFromServer);
    sendMessageToClients(messageFromServer);
}

void TCPServer::startAccept()
{
    auto newClient = std::make_shared<asio::ip::tcp::socket>(_ioContext);

    _acceptor.async_accept(*newClient, [this, newClient](const asio::error_code& ec) {
        if (!ec) {
            _clientsConnected.push_back(newClient);
            std::cout << "Accepted connection from: " << newClient->remote_endpoint() << std::endl;
            handleRead(newClient);
            // handleSend();
            startAccept();
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
}
