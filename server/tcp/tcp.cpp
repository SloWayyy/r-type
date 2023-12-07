/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcp.hpp"

Tcp_server::Tcp_server(char const *port) : _port(std::atoi(port)), _endpoint(asio::ip::tcp::v6(), std::atoi(port)), _acceptor(_ioContext, _endpoint), buffer()
{
}

std::vector<std::shared_ptr<asio::ip::tcp::socket>> &Tcp_server::getClientsConnected()
{
    return this->_clientsConnected;
}

asio::io_context &Tcp_server::getIoContext()
{
    return this->_ioContext;
}

asio::error_code &Tcp_server::getEc()
{
    return this->_ec;
}

asio::ip::tcp::endpoint &Tcp_server::getEndpoint()
{
    return this->_endpoint;
}

asio::ip::tcp::acceptor &Tcp_server::getAcceptor()
{
    return this->_acceptor;
}

asio::streambuf &Tcp_server::getBuffer()
{
    return this->buffer;
}

void Tcp_server::createSocket()
{
    this->_acceptor.listen();
    std::cout << "Server socket created and bound to port " << this->_port << std::endl;
}

void Tcp_server::handleRead(std::shared_ptr<asio::ip::tcp::socket> client)
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

void Tcp_server::startAccept()
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

void Tcp_server::run()
{
    startAccept();
    _ioContext.run();
}
