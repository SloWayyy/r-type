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

class Tcp_server {
    public:
        Tcp_server(char const *port);
        ~Tcp_server() = default;
        std::vector<std::shared_ptr<asio::ip::tcp::socket>> &getClientsConnected();
        asio::io_context &getIoContext();
        asio::error_code &getEc();
        asio::ip::tcp::endpoint &getEndpoint();
        asio::ip::tcp::acceptor &getAcceptor();
        asio::streambuf &getBuffer();
        void createSocket();
        void startAccept();
        void handleRead(std::shared_ptr<asio::ip::tcp::socket> client);
        void run();
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
