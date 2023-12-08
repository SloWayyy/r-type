/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#ifndef TCP_SERVER_HPP_
    #define TCP_SERVER_HPP_

    #include <asio.hpp>
    #include <iostream>

class TCPServer {
    public:
        TCPServer(std::size_t port);
        ~TCPServer() = default;
        void createSocket();
        void startAccept();
        void handleRead(std::shared_ptr<asio::ip::tcp::socket> client);
        void handleSend();
        void sendMessageToClients(const std::string &message);
    private:
        std::size_t _port;
        asio::io_context _ioContext;
        asio::ip::tcp::endpoint _endpoint;
        asio::ip::tcp::acceptor _acceptor;
        asio::streambuf buffer;
        std::vector<std::shared_ptr<asio::ip::tcp::socket>> _clientsConnected;
        asio::error_code _ec;
    protected:
};

#endif /* !TCP_SERVER_HPP_ */
