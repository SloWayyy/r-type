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
#include <any>
#include <typeindex>
#include <unordered_map>
#include <optional>

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

class UDPClient {
    public:
        UDPClient(std::size_t port, std::string ip);
        ~UDPClient();
        void start_receive();
        void handle_receive(const asio::error_code &error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code &error, std::size_t bytes_transferred);
        template <typename T>
        void send(const T &component, uint32_t entity_id, PacketType packet_type);
        void run();
        template <typename T>
        std::string pack(const T &component, uint32_t entity_id, PacketType packet_type);
        std::string unpack(Packet &packet);
        std::thread _thread;

    private:
        std::size_t _port;
        asio::ip::udp::endpoint _endpointServer;
        asio::io_context _io_context;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> _recv_buffer;
        long _last_timestamp = 0;
        uint32_t _magic_number = 4242;
};

#endif /* !UDP_HPP_ */
