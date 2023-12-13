/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

struct Position {
    float x;
    float y;
};

UDPClient::UDPClient(std::size_t port, std::string ip)
    : _port(port),
    _endpointServer(asio::ip::make_address(ip), 4242),
    socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0))
{
    start_receive();
    send("salut serveur", _endpointServer);
}

void UDPClient::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&UDPClient::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

std::unordered_map<uint32_t, std::type_index> _typeIndex = {
    {526835, typeid(Position)}
};

std::string UDPClient::unpack(Packet &packet)
{
    try {
        std::memcpy(&packet, _recv_buffer.data(), sizeof(Packet));
        std::string component;
        std::memcpy(&component, _recv_buffer.data() + sizeof(Packet), sizeof(component));
        return component;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return nullptr;
    }
}

void UDPClient::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "bytes transferred: " << bytes_transferred << std::endl;
            // const uint32_t* bufferPtr = reinterpret_cast<const uint32_t*>(recv_buffer_.data());
            // std::cout << "bufferPtr: " << recv_buffer_.data() << std::endl;

    // Copie les données dans la structure
        Packet packet;
        // std::string compo = unpack(packet);
        // auto compo = static_cast<typeid(Position)>(compo);
        // // if (component == nullptr)
        // //     return;
        // std::cout << "TOTO packet.magic_number: " << packet.magic_number << " packet.entity_id: " << packet.entity_id << " packet.type_index: " << packet.type_index << "x: " << std::endl;
        std::memcpy(&packet, _recv_buffer.data(), sizeof(packet));
        std::cout << "TOTO packet.magic_number: " << packet.magic_number << " packet.entity_id: " << packet.entity_id << " packet.type_index: " << packet.type_index << std::endl;
        Position pos ;
        std::memcpy(&pos, _recv_buffer.data() + sizeof(packet), sizeof(pos));
        // = std::any_cast<Position>(packet.component);

        std::cout << "pos.x: " << pos.x << " pos.y: " << pos.y << std::endl;

        start_receive();
    }
}

void UDPClient::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    _io_context.run();
}

void UDPClient::send(std::string message, asio::ip::udp::endpoint endpoint)
{
    socket_.send_to(asio::buffer(message), endpoint);
}

void UDPClient::handle_send(std::shared_ptr<std::string>, const asio::error_code&, std::size_t)
{
    std::cout << "Message envoyé" << std::endl;
}
