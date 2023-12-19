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
#include <mutex>
#include <random>
#include <array>
#include "../../ecs/registry/registry.hpp"

enum PacketType {
    DATA_PACKET = '0',
    REPEAT_PACKET = '1',
    RESPONSE_PACKET = '2',
    NEW_CONNECTION = '3',
    ASK_ENTITY = '4',
};

struct Packet {
    uint32_t magic_number;
    PacketType packet_type;
    long timestamp;
    uint32_t entity_id;
    uint32_t type_index;
    std::array<char, 37> uuid;

    void display_packet()
    {
        std::cout << "magic_number: " << magic_number << std::endl;
        std::cout << "packet_type: " << packet_type << std::endl;
        std::cout << "timestamp: " << timestamp << std::endl;
        std::cout << "entity_id: " << entity_id << std::endl;
        std::cout << "type_index: " << type_index << std::endl;
        std::cout << "uuid: " << uuid.data() << std::endl;
    }
};

class UDPClient {
    public:
        UDPClient(std::size_t port, std::string ip, registry &reg);
        ~UDPClient();
        void start_receive();
        void handle_receive(const asio::error_code &error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code &error, std::size_t bytes_transferred);
        template <typename T>
        void send(T const &component, uint32_t entity_id, PacketType packet_type);
        void run();
        std::array<char, 37> generate_uuid();
        template <typename T>
        std::vector<uint8_t> pack(T const& component, uint32_t entity_id, PacketType packet_type);
        std::vector<uint8_t> unpack(Packet &packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred);
        std::vector<std::pair<Packet, std::vector<uint8_t>>> _queue;
        std::mutex mtx;
        void saveData();
        void askEntity();
    private:
        int myEntityId = -1;
        std::thread _thread;
        std::size_t _port;
        asio::ip::udp::endpoint _endpointServer;
        asio::io_context _io_context;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<uint8_t, 1024> _recv_buffer;
        long _last_timestamp = 0;
        uint32_t _magic_number = 4242;
        registry &reg;
};

#include "udp.cpp"

#endif /* !UDP_HPP_ */
