/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcp.hpp"

TCPServer::TCPServer(std::size_t port) : _port(port), _endpoint(asio::ip::tcp::v6(), port), _acceptor(_ioContext, _endpoint), buffer()
{
    std::cout << "Constructing TCPServer..." << _port << std::endl;
    std::cout << _endpoint.address() << _endpoint.port() << std::endl;
}

void TCPServer::createSocket()
{
    std::cout << "Creating server socket..." << std::endl;
    this->_acceptor.listen();
    std::cout << "Server socket created and bound to port " << this->_port << std::endl;
}

void TCPServer::handleRead(std::shared_ptr<asio::ip::tcp::socket> client)
{
    asio::async_read_until(*client, buffer, "\n", [this, client](const asio::error_code& ec, std::size_t bytesRead) {
        if (!ec) {
            std::istream input(&this->buffer);
            std::string data;
            std::getline(input, data);
            std::cout << "Client: " << client->remote_endpoint() << " Sent: " << data << std::endl;
            std::string response = "Server response: " + data + "\n";
            asio::write(*client, asio::buffer(response));
            handleRead(client);
        } else {
            std::cerr << "Error reading data from client " << client->remote_endpoint() << ": " << ec.message() << std::endl;
            _clientsConnected.erase(std::remove(_clientsConnected.begin(), _clientsConnected.end(), client), _clientsConnected.end());
        }
    });
}

void TCPServer::startAccept()
{
    auto newClient = std::make_shared<asio::ip::tcp::socket>(_ioContext);

    _acceptor.async_accept(*newClient, [this, newClient](const asio::error_code& ec) {
        if (!ec) {
            _clientsConnected.push_back(newClient);
            std::cout << "Accepted connection from: " << newClient->remote_endpoint() << std::endl;
            handleRead(newClient);
            startAccept();
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
}

void TCPServer::run()
{
    startAccept();
    _ioContext.run();
}
