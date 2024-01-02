/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#include "tcpServer.hpp"

TCPServer::TCPServer(std::size_t port, std::size_t portUDP, std::string ip)
    : _port(port), _portUDP(portUDP), _ioContext(), buffer(), _ip(ip),
      _endpoint(asio::ip::make_address(ip), port),
      _acceptor(_ioContext, _endpoint)
{
    this->_adminPassword = "admin";
    this->OpenAndReadAdmin("../network/tcpServer/passwordAdmin.txt");
    std::cout << "Admin password: " << this->_adminPassword << std::endl;
    this->OpenAndReadScoreboard("../network/tcpServer/scoreboard.txt");
    this->createSocket();
    this->startAccept();
    this->_thread = std::thread(&TCPServer::run, this);
}

TCPServer::~TCPServer() {
    this->_acceptor.close();
    this->_ioContext.stop();
    this->_thread.join(); 
}

int TCPServer::OpenAndReadAdmin(std::string file)
{
    std::fstream ifs;
    std::string buffer;

    ifs.open(file, std::fstream::in);
    if (ifs.is_open()) {
        try {
            std::getline(ifs, buffer);
            this->_adminPassword = buffer;
            if (buffer.empty())
                this->_adminPassword = "admin";
        } catch (const std::exception& e) {
            this->_adminPassword = "admin";
            std::cerr << "Error: " << e.what() << std::endl;
            return (-1);
        }
    } else {
        std::cerr << file << ": No such file or directory" << std::endl;
        return (-1);
    }
    ifs.close();
    return (0);
}

int TCPServer::OpenAndReadScoreboard(std::string file)
{
    std::fstream ifs;
    std::string buffer;

    ifs.open(file, std::fstream::in);
    if (ifs.is_open()) {
        while (std::getline(ifs, buffer)) {
            this->_scoreboard.push_back(std::make_pair(std::stoi(buffer.substr(0, buffer.find(":"))), std::stoi(buffer.substr(buffer.find(":") + 1, buffer.size()))));
        }
    } else {
        std::cerr << file << ": No such file or directory" << std::endl;
        return (-1);
    }
    // for (auto &i : _scoreboard) {
    //     std::cout << "SCOREBOARD: " << i.first << " " << i.second << std::endl;
    // }
    ifs.close();
    return (0);
}

int TCPServer::createSocket()
{
    std::cout << "Creating server socket (TCP)..." << std::endl;
    try {
        this->_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
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
            this->_ClientMessages.push_back(data);
            handleRead(client);
        } else {
            std::cerr << "client " << client->remote_endpoint() << " is disconnected." << std::endl;
            _clientsInfo.erase(client->remote_endpoint().port());
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
    this->_acceptor.async_accept(*newClient, [this, newClient](const asio::error_code& ec) {
        if (!ec) {
            std::cout << "New connection TCP: " << newClient->remote_endpoint() << std::endl;
            handleRead(newClient);
            this->_clientsInfo[newClient->remote_endpoint().port()] = newClient;
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
