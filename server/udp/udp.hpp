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
#include <any>
#include <typeindex>
#include <ctime>

enum PacketType {
    DATA_PACKET = '0',
    REPEAT_PACKET = '1',
    RESPONSE_PACKET = '2',
    NEW_CONNECTION = '3',
};
struct Packet
{
    uint32_t magic_number;
    PacketType packet_type;
    long timestamp;
    uint32_t entity_id;
    uint32_t type_index;
};

class UDPServer {
    public:
        UDPServer(std::size_t port, std::string ip);
        void start_receive();
        void handle_receive(const asio::error_code &error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code &error, std::size_t bytes_transferred);
        void response(std::string message);
        void send(std::string message, asio::ip::udp::endpoint endpoint);
        template <typename T>
        void sendToAll(const T &component, uint32_t entity_id, PacketType packet_type = 0);
        void run();
        size_t getPort() const;
        template <typename T>
        std::string pack(const T &component, uint32_t entity_id, PacketType packet_type = 0);
        std::string unpack(Packet &packet, std::array<uint8_t, 1024> query);
        std::unordered_map<size_t, asio::ip::udp::endpoint> _clientsUDP;
        std::vector<std::pair<asio::ip::udp::endpoint, std::string>> _queries;

    private:
        std::size_t _port;
        asio::io_context _io_context;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<uint8_t, 1024> _recv_buffer;
        asio::ip::udp::endpoint _endpointClient;
        uint32_t _magic_number;
    };

#endif /* !UDP_HPP_ */
