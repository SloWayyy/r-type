/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#ifndef UDP_HPP_
#define UDP_HPP_

#include "../../ecs/registry/registry.hpp"
#include <any>
#include <array>
#include <asio.hpp>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <typeindex>
#include <unordered_map>

enum PacketType {
    DATA_PACKET = '0',
    REPEAT_PACKET = '1',
    RESPONSE_PACKET = '2',
    NEW_CONNECTION = '3',
    NEW_PLAYER = '4',
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

class Udp {
    public:

        // constructor / destructor
        Udp(std::size_t port, std::string ip, registry &reg);
NEW_PLAYER        template <typename ...Args>
        void sendToAll(PacketType packet_type = '0', Args ...args);

        template <typename... Args>
        void sendClientToServer(Args... args);
        template <typename... Args>
        void sendServerToClient(PacketType packet_type, asio::ip::udp::endpoint endpoint, Args... args);

        void run();
        void updateSparseArray(bool isClient);
        std::array<char, 37> generate_uuid();


        template <typename T>
        std::vector<uint8_t> createPacket(PacketType packet_type, T const &component, uint32_t entity_id);
        std::vector<uint8_t> createPacket(std::vector<uint8_t> component, Packet packet);
        std::vector<uint8_t> createPacket(PacketType packet_type, uint32_t entity_id);
        std::array<uint8_t, sizeof(Packet)> createPacket(Packet packet);
        std::vector<uint8_t> unpack(Packet &packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred);

        size_t getPort() const {return _port;};
        uint32_t getEntityId() const {return _entity_id;};

        std::unordered_map<size_t, asio::ip::udp::endpoint> _clientsUDP;
        std::vector<std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>>> _queueSendPacket;
        std::vector<std::pair<Packet, std::vector<uint8_t>>> _queue;
        std::mutex mtx;
        std::mutex mtxSendPacket;
        std::mutex mtxQueue;

    private:
        std::thread _thread;
        std::size_t _port;
        asio::ip::udp::endpoint _endpointServer;
        asio::io_context _io_context;
        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<uint8_t, 1024> _recv_buffer;
        long _last_timestamp = 0;
        asio::ip::udp::endpoint _endpointClient;
        uint32_t _magic_number = 4242;
        uint32_t _entity_id = 0;
        registry &reg;
};

#include "udp.cpp"

#endif /* !UDP_HPP_ */