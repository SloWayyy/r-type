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
}

int TCPServer::createSocket()
{
    std::cout << "Creating server socket..." << std::endl;
    try {
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        this->_acceptor.listen();
    } catch (const asio::system_error& ec) {
        std::cerr << "Error opening socket: " << ec.what() << std::endl;
        return -1;
    }
    return 0;
}

void TCPServer::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_ioContext.run();
}

void TCPServer::handleRead(std::shared_ptr<asio::ip::tcp::socket> client)
{
    asio::async_read_until(*client, buffer, "\n", [this, client](const asio::error_code& ec, std::size_t bytesRead) {
        if (!ec) {
            std::istream input(&this->buffer);
            std::string data;
            std::getline(input, data);
            std::cout << "Client: " << client->remote_endpoint().address() << ":" << client->remote_endpoint().port() << " Send: " << data << std::endl;

            // if (data == "First co") {
            //     // sendMessageToAllClients("New client connected");
            //     sendMessageToAClient("New client connected", client);
            //     // sendMessageToClients("New client connected");
            // }

            handleRead(client);
        } else {
            std::cerr << "client " << client->remote_endpoint() << " is disconnected."  << std::endl;
            _clientsConnected.erase(std::remove(_clientsConnected.begin(), _clientsConnected.end(), client), _clientsConnected.end());
            sendMessageToAllClients("(RFC) PLAYER DISCONNECTED " + std::to_string(std::find (_clientsConnected.begin(), _clientsConnected.end(), client) - _clientsConnected.begin()));
        }
    });
}


void TCPServer::sendMessageToAllClients(const std::string &message)
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

void TCPServer::sendMessageToAClient(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> client)
{
    client->async_send(asio::buffer(message + "\n"), [this, client](const asio::error_code& ec, std::size_t bytesSent) {
        if (!ec) {
            std::cout << "Message sent to client " << client->remote_endpoint() << std::endl;
        } else {
            std::cerr << "Error sending message to client " << client->remote_endpoint() << ": " << ec.message() << std::endl;
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
                std::cout << "Sending welcome message to client..." << std::endl;
                sendMessageToAClient("(RFC) HELLO PORT UPD " + std::to_string(_port) + " YOUR ID " + std::to_string(std::find (_clientsConnected.begin(), _clientsConnected.end(), newClient) - _clientsConnected.begin()), newClient);
                std::cout << _clientsConnected.size() << std::endl;
                for (const auto &client : _clientsConnected) 
                    if (client != newClient)
                        sendMessageToAClient("(RFC) PLAYER ID " + std::to_string(std::find (_clientsConnected.begin(), _clientsConnected.end(), client) - _clientsConnected.begin()), newClient);
                for (const auto &client : _clientsConnected) {
                    sendMessageToAClient("(RFC) PLAYER ID " + std::to_string(std::find (_clientsConnected.begin(), _clientsConnected.end(), newClient) - _clientsConnected.begin()), client);
                }
            startAccept();
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
}
