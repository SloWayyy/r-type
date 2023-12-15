/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"
#include <typeindex>
#include <random>
#include <unordered_map>

struct Position {
    float x;
    float y;
};

UDPServer::UDPServer(std::size_t port, std::string ip)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port)), _magic_number(4242)
{
    this->_port = socket_.local_endpoint().port();
    start_receive();
}

void UDPServer::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&UDPServer::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

std::unordered_map<uint32_t, std::type_index> _typeIndex = {
    {2, typeid(Position)}};

template <typename T>
std::string UDPServer::pack(const T &component, uint32_t entity_id, PacketType packet_type)
{
    std::type_index targetType = typeid(T);
    int type_index = -1;

    for (const auto &entry : _typeIndex) {
        if (entry.second == targetType) {
            type_index = entry.first;
            break;
        }
    }
    if ((type_index) == -1) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return "";
    } else {
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, static_cast<u_int32_t>(type_index)};
        try {
            return std::string(reinterpret_cast<char *>(&packet),
            sizeof(packet)) + std::string(reinterpret_cast<const char *>(&component),
            sizeof(component));
        } catch (const std::exception &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return "";
        }
    }
}

void UDPServer::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "bytes transferred to serv: " << bytes_transferred << std::endl;
        Packet receivedPacket;
        std::memcpy(&receivedPacket, _recv_buffer.data(), sizeof(Packet));
        std::cout << "Packet Type : " << receivedPacket.packet_type << std::endl;
        Position pos = {15.0, 15.0};
        sendToAll(pos, 1 , DATA_PACKET);
        start_receive();
    }
}

void UDPServer::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_io_context.run();
}

void UDPServer::response(std::string message)
{
    std::shared_ptr<std::string> message2(new std::string(message));
    socket_.async_send_to(asio::buffer(*message2), remote_endpoint_,
                          std::bind(&UDPServer::handle_send, this, message2,
                                    std::placeholders::_1,
                                    std::placeholders::_2));
    std::cout << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;
}

void UDPServer::send(std::string message, asio::ip::udp::endpoint endpoint)
{
    try {
        socket_.send_to(asio::buffer(message), endpoint);
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

template <typename T>
void UDPServer::sendToAll(const T &component, uint32_t entity_id, PacketType packet_type)
{
    std::string data = pack(component, entity_id, packet_type);
    if (data.empty())
        return;
    try {
        for (const auto &client : _clientsUDP) {
            socket_.send_to(asio::buffer(data), client.second);
        }
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void UDPServer::handle_send(std::shared_ptr<std::string> message, const asio::error_code &error, std::size_t bytes_transferred)
{
    std::cout << "Message envoyé" << std::endl;
}

size_t UDPServer::getPort() const
{
    return this->_port;
}
