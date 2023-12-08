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
    #include <functional>

class TCPClient {
    public:
        TCPClient(std::size_t port);
        ~TCPClient() = default;
        void createClient();
        void sendToServer();
        void readFromServer();
        void receiveFromServer();
        void handle_send(const asio::error_code &error, std::size_t bytes_transferred);
        void handleReceive();
    private:
        std::size_t _port;
        asio::io_context _ioContext;
        asio::ip::tcp::socket _socket;
        asio::ip::tcp::resolver _resolver;
        asio::ip::tcp::endpoint _endpoint;
        asio::streambuf buffer;
    protected:
};

#endif /* !TCP_SERVER_HPP_ */
