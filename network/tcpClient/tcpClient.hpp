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
    #include "../../ecs/registry/registry.hpp"

class TCPClient {
    public:
        TCPClient(std::size_t port, std::string ip, registry &reg);
        ~TCPClient();
        void run();
        std::thread _thread;
        void startAsyncOperations();
        void sendToServer(std::string message);

    private:
        void createClient();
        void handleSend(const asio::error_code &error, std::size_t bytes_transferred);
        void sendToServer();
        void handleReceive();
        std::vector<std::string> getServerMessages();
        std::size_t _port;
        asio::io_context _ioContext;
        asio::ip::tcp::socket _socket;
        asio::ip::tcp::resolver _resolver;
        asio::ip::tcp::endpoint _endpoint;
        std::string _ip;
        asio::streambuf buffer;
        std::vector<std::string> _ServerMessages;
        registry &reg;
};

#endif /* TCP_CLIENT_HPP_ */
