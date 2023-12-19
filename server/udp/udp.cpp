/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include <typeindex>
#include <random>
#include <unordered_map>

struct Position2 {
    float x;
    float y;
};

bool operator==(const Packet &lhs, const Packet &rhs)
{
    return lhs.magic_number == rhs.magic_number
            && lhs.packet_type == rhs.packet_type
            && lhs.timestamp == rhs.timestamp
            && lhs.entity_id == rhs.entity_id
            && lhs.type_index == rhs.type_index;
}

UDPServer::UDPServer(std::size_t port, std::string ip, registry &reg)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), 0)), _magic_number(4242), reg(reg)
{
    try {
        this->socket_ = asio::ip::udp::socket(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port));
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP creating socket" << ec.what() << std::endl;
        exit(84);
    }
    this->_port = socket_.local_endpoint().port();
    std::cout << "UDP port: " << this->_port << std::endl;
    _thread = std::thread(&UDPServer::run, this);
    start_receive();
}

UDPServer::~UDPServer()
{
    std::cout << "Server is closing" << std::endl;
    _thread.join();
}

void UDPServer::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&UDPServer::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
}

template <typename T>
std::vector<uint8_t> UDPServer::pack(T const& component, uint32_t entity_id, PacketType packet_type)
{
    uint32_t type_index = 0;

    for (; type_index < reg._typeIndex.size(); type_index++) {
        if (reg._typeIndex[type_index] == std::type_index(typeid(component)))
            break;
    }

    if (type_index == reg._typeIndex.size()) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return {};
    } else {
        std::array<char, 37> uuid = generate_uuid();
        std::cout << "UUID CLIENT du packet: " << uuid.data() << std::endl;
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid};
        try {
            std::vector<uint8_t> result;
            result.resize(sizeof(Packet) + sizeof(T));
            result.insert(result.end(), (uint8_t *)&packet, (uint8_t *)&packet + sizeof(Packet));
            result.insert(result.end(), (uint8_t *)&component, (uint8_t *)&component + sizeof(T));
            return result;
        } catch (const std::exception &e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return {};
        }
    }
}

std::array<char, 37> UDPServer::generate_uuid() {
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

void UDPServer::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        Packet receivedPacket;
        std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);
        std::cout << "component size"<< std::endl;
        if (receivedComponent.size() == 0) {
            start_receive();
            return;
        }
        if (receivedPacket.packet_type == NEW_CONNECTION) {
            _clientsUDP[receivedPacket.entity_id] = remote_endpoint_;
            std::cout << "New player connected from " << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;
            sendToAll(receivedPacket, 0, NEW_CONNECTION);
            start_receive();
            return;
        }
        if (receivedPacket.magic_number != _magic_number) {
            std::cerr << "ERROR: magic number not valid in received packet" << std::endl;
            start_receive();
            return;
        }
        if (receivedPacket.packet_type == RESPONSE_PACKET) {
            for (const auto &query : _queueSendPacket) {
                Packet queryPacket;
                std::memcpy(&queryPacket, query.second.data(), sizeof(Packet));
                if (receivedPacket.uuid == queryPacket.uuid) {
                    mtxSendPacket.lock();
                    _queueSendPacket.erase(std::remove(_queueSendPacket.begin(), _queueSendPacket.end(), query), _queueSendPacket.end());
                    mtxSendPacket.unlock();
                }
            }
            start_receive();
        }
        mtxQueue.lock();
        _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
        mtxQueue.unlock();
        start_receive();
    }
}

void UDPServer::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_io_context.run();
}


void UDPServer::send(std::vector<uint8_t> message, asio::ip::udp::endpoint endpoint)
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
    std::vector<uint8_t> data = pack(component, entity_id, packet_type);

    if (data.empty())
        return;
    try {
        for (const auto &client : _clientsUDP) {
            std::cout << "Message sent to client UDP: " << client.first << std::endl;
            socket_.send_to(asio::buffer(data), client.second);
            if (packet_type == DATA_PACKET) {
                data[4] = RESPONSE_PACKET;
                mtxSendPacket.lock();
                _queueSendPacket.push_back(std::make_pair(client.second, data));
                mtxSendPacket.unlock();
            }
        }
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

std::vector<uint8_t> UDPServer::unpack(Packet &packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred)
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

size_t UDPServer::getPort() const
{
    return this->_port;
}
