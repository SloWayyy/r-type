/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

UDPClient::UDPClient(std::size_t port, std::string ip, registry &reg) :
    _endpointServer(asio::ip::make_address(ip), 4242),
    socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)), _last_timestamp(0), reg(reg)
{
    _thread = std::thread(&UDPClient::run, this);
    _port = socket_.local_endpoint().port();
    start_receive();
    send("pos", 0, NEW_CONNECTION);
}

UDPClient::~UDPClient()
{
    _thread.join();
}

void UDPClient::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&UDPClient::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

std::vector<uint8_t> UDPClient::unpack(Packet &packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred)
{
    try {
        std::memcpy(&packet, query.data(), sizeof(Packet));
        std::vector<uint8_t> component(query.begin() + sizeof(Packet), query.begin() + bytes_transferred);
        std::cout << "component size: " << component.size() << std::endl;
        return component;
    } catch (const std::exception &e) {
        std::cerr << "ERROR unpack: " << e.what() << std::endl;
        return {};
    }
}

void UDPClient::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "bytes transferred: " << bytes_transferred << std::endl;
        Packet packet;
        std::vector<uint8_t> receivedComponent = unpack(packet, _recv_buffer, bytes_transferred);
        if (receivedComponent.size() == 0) {
            start_receive();
            return;
        }
        if (packet.timestamp >= _last_timestamp) {
            _last_timestamp = packet.timestamp;
            mtx.lock();
            _queue.push_back(std::make_pair(packet, receivedComponent));
            mtx.unlock();
        } else {
            std::cout << "OUTDATED PACKET" << std::endl;
        }
        packet.packet_type = RESPONSE_PACKET;
        std::array<uint8_t, sizeof(Packet)> buffer;
        std::memcpy(buffer.data(), &packet, sizeof(Packet));
        std::cout << "j'ai renvoye le packet" << std::endl;
        socket_.send_to(asio::buffer(buffer), remote_endpoint_);
        start_receive();
    }
}

template <typename T>
std::vector<uint8_t> UDPClient::pack(T const& component, uint32_t entity_id, PacketType packet_type)
{
    uint32_t type_index = 0;

    for (; type_index < reg._typeIndex.size(); type_index++) {
        if (reg._typeIndex[type_index] == std::type_index(typeid(component)))
            break;
    }

    if (type_index == reg._typeIndex.size() and packet_type != NEW_CONNECTION) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return {};
    } else {
        std::array<char, 37> uuid = generate_uuid();
        std::cout << "UUID CLIENT du packet: " << uuid.data() << std::endl;
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid};
        packet.display_packet();
        std::cout << "packet size: " << sizeof(Packet) << std::endl;
        try {
            std::vector<uint8_t> result;
            result.resize(sizeof(Packet) + sizeof(T));
            const uint8_t *packetBytes = reinterpret_cast<const uint8_t *>(&packet);
            std::copy(packetBytes, packetBytes + sizeof(Packet), result.begin());
            const uint8_t *componentBytes = reinterpret_cast<const uint8_t *>(&component);
            std::copy(componentBytes, componentBytes + sizeof(T), result.begin() + sizeof(Packet));
            return result;
        } catch (const std::exception &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return {};
        }
    }
}

void UDPClient::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    _io_context.run();
}

template <typename T>
void UDPClient::send(const T &component, uint32_t entity_id, PacketType packet_type)
{
    std::vector<uint8_t> data = pack(component, entity_id, packet_type);

    if (data.size() == 0)
        return;
    try {
        std::cout << "Message sent to server UDP: " << " on adress " << _endpointServer.address() << " on port " << _endpointServer.port() << std::endl;
        socket_.send_to(asio::buffer(data), _endpointServer);
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void UDPClient::handle_send(std::shared_ptr<std::string>, const asio::error_code&, std::size_t)
{
    std::cout << "Message envoyé" << std::endl;
}

std::array<char, 37> UDPClient::generate_uuid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::array<char, 37> uuid;
    std::string hexChars = "0123456789abcdef";

    for (int i = 0; i < 37; i++) {
        uuid[i] = hexChars[dis(gen)];
    }
    return uuid;
}

void UDPClient::saveData()
{
    std::cout << "GET DATA CLIENT" << std::endl;
    for (int i = 0; i < _queue.size(); i++) {
        Packet packet = _queue[i].first;
        int size = _queue[i].second.size();
        char *packet2;
        std::memcpy(packet2, _queue[i].second.data(), size);
        reg.registerPacket(packet.type_index, packet.entity_id, packet2);
    }
}
