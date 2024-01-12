/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#ifndef UDP_HPP_
#define UDP_HPP_

#define CRYPT_KEY 3

#include "../../ecs/registry/registry.hpp"
#include "../../server/updateGame/updateGame.hpp"
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
    EVENT_PACKET = '4',
    DESTROY_ENTITY = '5',
    DEAD_ENTITY = '6',
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
    Udp(std::size_t port, std::string ip, registry& reg);
    Udp(std::string ip, registry& reg); // client
    ~Udp();

    void start_receive(bool isClient = false);
    void handleReceiveClient(const asio::error_code& error, std::size_t bytes_transferred); // client
    void handleNewConnection(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent);
    void handleTimestampUpdate(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent);
    void handleReceiveServer(const asio::error_code& error, std::size_t bytes_transferred);
    void processReceivedPacket(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent);
    void handleNewConnection(const Packet& receivedPacket);
    void handleResponsePacket(const Packet& receivedPacket);
    void handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred); // client
    int handleErrorReceive(const asio::error_code& error, std::vector<uint8_t> receivedComponent, Packet receivedPacket, bool isClient);
    void handleEvents(const std::vector<uint8_t>& receivedComponent);

    std::vector<uint8_t> cryptMessage(std::vector<uint8_t> message);

    template <typename... Args> void sendToAll(PacketType packet_type = '0', Args... args);

    template <typename... Args> void sendClientToServer(Args... args);
    template <typename... Args> void sendServerToClient(PacketType packet_type, Args... args);

    void sendServerToAClient(std::vector<uint8_t> data, asio::ip::udp::endpoint endpoint);
    void sendPlayerListToClient(std::vector<std::vector<uint8_t>> entities, Packet receivedPacket);

    void run();
    void updateSparseArray(bool isClient);
    std::array<char, 37> generate_uuid();

    template <typename T> std::vector<uint8_t> createPacket(PacketType packet_type, T const& component, uint32_t entity_id);
    template <typename T> std::vector<uint8_t> createPacket(T& event, uint32_t entity_id);
    std::vector<uint8_t> createPacket(std::vector<uint8_t> component, Packet packet);
    std::vector<uint8_t> createPacket(PacketType packet_type, uint32_t entity_id);
    std::array<uint8_t, sizeof(Packet)> createPacket(Packet packet);
    std::vector<uint8_t> unpack(Packet& packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred);

    size_t getPort() const { return _port; };
    uint32_t getEntityId() const { return _entity_id; };

    std::unordered_map<size_t, asio::ip::udp::endpoint> _clientsUDP;
    std::vector<std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>>> _queueSendPacket;
    std::vector<std::pair<Packet, std::vector<uint8_t>>> _queue;
    std::mutex mtx;
    std::mutex mtxSendPacket;
    std::mutex mtxQueue;
    std::vector<std::vector<std::vector<uint8_t>>> _sparseArray;
    std::mutex _eventmtx;
    std::vector<std::any> _eventQueue;

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
    int _entity_id = -1;
    registry& reg;
    UpdateGame updateGame;
    const std::map<std::size_t, std::function<void(const Packet&, const std::vector<uint8_t>&)>> ptr_fct;
};

#include "udp.cpp"

#endif /* !UDP_HPP_ */
