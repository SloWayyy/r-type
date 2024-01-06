/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcpServer.hpp"
#include <regex>

TCPServer::TCPServer(std::size_t port, std::size_t portUDP, std::string ip)
    : _port(port)
    , _portUDP(portUDP)
    , _ioContext()
    , buffer()
    , _ip(ip)
    , _endpoint(asio::ip::make_address(ip), port)
    , _acceptor(_ioContext, _endpoint)
{
    this->createSocket();
    startAccept();
    _thread = std::thread(&TCPServer::run, this);
}

TCPServer::~TCPServer()
{
    _acceptor.close();
    _ioContext.stop();
    _thread.join();
}

int TCPServer::createSocket()
{
    std::cout << "Creating server socket (TCP)..." << std::endl;
    try {
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        this->_acceptor.listen();
    } catch (const asio::system_error& ec) {
        std::cerr << "Error opening socket (TCP): " << ec.what() << std::endl;
        return -1;
    }
    return 0;
}

void TCPServer::run()
{
    std::cout << "Server (TCP) is running on port " << this->_port << std::endl;
    std::cout << "Server is running on port (UDP) " << this->_portUDP << std::endl;
    this->_ioContext.run();
}

void TCPServer::handleRead(std::shared_ptr<asio::ip::tcp::socket> client)
{
    asio::async_read_until(*client, buffer, "\n", [this, client](const asio::error_code& ec, std::size_t) {
        if (!ec) {
            std::istream input(&this->buffer);
            std::string data;
            std::getline(input, data);
            std::cout << "Client: " << client->remote_endpoint().address() << ":" << client->remote_endpoint().port() << " Send: " << data
                    << std::endl;
            if (data.size() > 10 && data.substr(0, 10) == "(RFC) 210 ") {
                data.insert(10, "player_id(" + std::to_string(client->remote_endpoint().port()) + ") ");
                static std::regex pattern("player_id\\((\\d+)\\)\\s*/(.*)");

                std::smatch match;
                if (std::regex_search(data, match, pattern)) {
                    std::string playerIdStr = match[1].str();
                    try {
                        int playerId = std::stoi(playerIdStr);
                        std::string restOfMessage = match[2].str();
                        if (std::strncmp(match[2].str().c_str(), "kick ", 5) == 0) {
                            std::cout << "KICKING PLAYER " << match[2].str()[5] << std::endl;
                            int index = match[2].str()[5] - '0';
                            size_t id = getClientByIndex(index);

                            if (id != PLAYER_NOT_FOUND) {
                                // _clientsInfo.erase(id);
                                // return;
                                closeConnection(_clientsInfo[id]);
                                return;
                            }
                        }
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Erreur de conversion en entier : " << e.what() << std::endl;
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Dépassement de capacité de l'entier : " << e.what() << std::endl;
                    }
                _mtx.lock();
                this->_ClientMessages.push_back(data);
                _mtx.unlock();
                }
            }
            handleRead(client);
        } else {
            std::cout << "JE RENTRE LAAA" << std::endl;
            std::cerr << "client " << client->remote_endpoint() << " is disconnected." << std::endl;
            _mtx.lock();
            _clientsInfo.erase(client->remote_endpoint().port());
            _mtx.unlock();
            sendMessageToAllClients("(RFC) PLAYER_ID " + std::to_string(client->remote_endpoint().port()) + " DISCONNECTED ");
        }
    });
}

void TCPServer::sendMessageToAllClients(const std::string& message)
{
    try {
        for (const auto& client : _clientsInfo) {
            client.second->async_send(asio::buffer(message + "\n"), [this, client](const asio::error_code& ec, std::size_t) {
                if (!ec) {
                    std::cout << "Message sent to client " << client.second->remote_endpoint() << std::endl;
                } else {
                    std::cerr << "Error sending message to client " << client.second->remote_endpoint() << ": " << ec.message() << std::endl;
                }
            });
        }
    } catch (const asio::system_error& ec) {
        std::cerr << "Error sending message to all clients: " << ec.what() << std::endl;
    }
}

void TCPServer::sendMessageToAClient(const std::string& message, std::shared_ptr<asio::ip::tcp::socket> client)
{
    try {
        client->async_send(asio::buffer(message + "\n"), [this, client](const asio::error_code& ec, std::size_t) {
            if (!ec) {
                std::cout << "Message sent to client " << client->remote_endpoint() << std::endl;
            } else {
                std::cerr << "Error sending message to client " << client->remote_endpoint() << ": " << ec.message() << std::endl;
            }
        });
    } catch (const asio::system_error& ec) {
        std::cerr << "Error sending message to all clients: " << ec.what() << std::endl;
    }
}

void TCPServer::startAccept()
{
    auto newClient = std::make_shared<asio::ip::tcp::socket>(_ioContext);

    _acceptor.async_accept(*newClient, [this, newClient](const asio::error_code& ec) {
        if (!ec) {
            std::cout << "New connection TCP: " << newClient->remote_endpoint() << std::endl;
            handleRead(newClient);
            _clientsInfo[newClient->remote_endpoint().port()] = newClient;
            std::cout << "Accepted connection from: " << newClient->remote_endpoint() << std::endl;
            sendMessageToAClient(
                "(RFC) PORT_UPD " + std::to_string(_port) + " YOUR_ID " + std::to_string(newClient->remote_endpoint().port()), newClient);
            sendMessageToAllClients("(RFC) PLAYER_ID " + std::to_string(newClient->remote_endpoint().port()));
            for (const auto& client : _clientsInfo) {
                if (client.first != newClient->remote_endpoint().port()) {
                    sendMessageToAClient("(RFC) PLAYER_ID " + std::to_string(client.first), newClient);
                }
            }
            startAccept();
        } else {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
        }
    });
}

void TCPServer::closeConnection(std::shared_ptr<asio::ip::tcp::socket> client) {
    std::cout << "Closing connection for client " << client->remote_endpoint() << std::endl;
    auto port = client->remote_endpoint().port();
    try {
        if (client->is_open()) {
            std::cout << "Connection closed for client " << client->remote_endpoint() << std::endl;
            client->close();
        } else {
            std::cout << "Connection is not open for client " << client->remote_endpoint() << std::endl;
        }
    } catch (const asio::system_error& ec) {
        std::cerr << "Error closing connection for client " << client->remote_endpoint() << ": " << ec.what() << std::endl;
    }
    // throw asio::system_error();

    // auto it = _clientsInfo.find(port);
    // if (it != _clientsInfo.end()) {
    //     std::cout << "JE SUPPRIME LE CLIENT" << std::endl;
    //     _clientsInfo.erase(it);
    // }
    // for (const auto& clientInfo : _clientsInfo) {
    //     if (clientInfo.second == client) {
    //         _clientsInfo.erase(clientInfo);
    //         // std::cout << "Client " << client->remote_endpoint() << " is disconnected." << std::endl;
    //     }
    // }
    // _clientsInfo.remove(client->remote_endpoint().port());
    // _clientsInfo.erase(static_cast<size_t>(port));
    // sendMessageToAllClients("(RFC) PLAYER_ID " + std::to_string(port) + " DISCONNECTED ");
}