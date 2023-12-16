/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** tcp.hpp
*/

#ifndef TCP_CLIENT_HPP_
    #define TCP_CLIENT_HPP_

    #include <asio.hpp>
    #include <iostream>
    #include <thread>
    #include <functional>

class TCPClient {
    public:
        TCPClient(std::size_t port, std::string ip);
        void run();
    private:
        void createClient();
        void handleSend(const asio::error_code &error, std::size_t bytes_transferred);
        void sendToServer();
        void handleReceive();
        void startAsyncOperations();
        std::vector<std::string> getServerMessages();

        std::size_t _port;
        asio::io_context _ioContext;
        asio::ip::tcp::socket _socket;
        asio::ip::tcp::resolver _resolver;
        asio::ip::tcp::endpoint _endpoint;
        std::string _ip;
        asio::streambuf buffer;
        std::vector<std::string> _ServerMessages;
};

#endif /* TCP_CLIENT_HPP_ */
