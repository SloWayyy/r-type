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
        // std::cout << "TOTO packet.magic_number: " << packet.magic_number << " packet.entity_id: " << packet.entity_id << " packet.type_index: " << packet.type_index << std::endl;
        return component;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return nullptr;
    }
}


//         using reference_type = value_type &;
//         using const_reference_type = value_type const &;
// using value_type = std::optional<Component>;
// using container_t = std::vector<value_type>;
// std::unordered_map<std::type_index, std::any> _components;
        // Packet packet;
        // std::string compo = unpack(packet);
        // std::type_index Index = _typeIndex[packet.type_index];
        // container_t components = _components[Index];
        // components[packet.entity_id] = std::any_cast<value_type>(compo);

void UDPClient::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "bytes transferred: " << bytes_transferred << std::endl;
        Packet packet;
        std::memcpy(&packet, _recv_buffer.data(), sizeof(packet));
        std::cout << "TOTO packet.magic_number: " << packet.magic_number << " packet.entity_id: " << packet.entity_id << " packet.type_index: " << packet.type_index << std::endl;
        Position pos;
        std::memcpy(&pos, _recv_buffer.data() + sizeof(packet), sizeof(pos));
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
