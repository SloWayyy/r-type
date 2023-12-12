/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#ifndef UDP_HPP_
#define UDP_HPP_

    #include <asio.hpp>
    #include <iostream>
    #include <array>
    #include <functional>
    #include <memory>
    #include <unordered_map>

class UDPServer {
    public:
        UDPServer(std::size_t port, std::string ip);
        void start_receive();
        void handle_receive(const asio::error_code& error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred);
        void response(std::string message);
        void send(std::string message, asio::ip::udp::endpoint endpoint);
        void run();
        size_t getPort() const;
        std::unordered_map<size_t, asio::ip::udp::endpoint> _clientsUDP;
    private:
        std::size_t _port;
        asio::io_context _io_context;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
        asio::ip::udp::endpoint _endpointClient;
};

#endif /* !UDP_HPP_ */
