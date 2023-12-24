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
        this->socket_  = asio::ip::udp::socket(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port));
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP binding socket: " << ec.what() << std::endl;
        std::exit(84);
    }
    this->_port = socket_.local_endpoint().port();
    _thread = std::thread(&UDPServer::run, this);
    start_receive();
}

UDPServer::UDPServer(std::size_t port, std::string ip, registry &reg, bool client) :
    _endpointServer(asio::ip::make_address(ip), 4242),
    socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)), _last_timestamp(0), reg(reg)
{
    _thread = std::thread(&UDPServer::run, this);
    _port = socket_.local_endpoint().port();
    start_receive_client();
    send_client("pos", 0, NEW_CONNECTION);
}

UDPServer::~UDPServer()
{
    std::cout << "Server is closing" << std::endl;
    _thread.join();
}

void UDPServer::start_receive_client()
{
    socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&UDPServer::handle_receive_client, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
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
std::vector<uint8_t> UDPServer::pack_client(T const& component, uint32_t entity_id, PacketType packet_type)
{
    uint32_t type_index = 0;

    for (; type_index < reg._typeIndex.size(); type_index++) {
        if (reg._typeIndex[type_index] == std::type_index(typeid(component)))
            break;
    }

    if (type_index == reg._typeIndex.size() && packet_type != NEW_CONNECTION) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return {};
    } else {
        std::array<char, 37> uuid = generate_uuid();
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid};
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

template <typename T>
std::vector<uint8_t> UDPServer::pack(T const& component, uint32_t entity_id, PacketType packet_type)
{
    uint32_t type_index = 0;

    for (; type_index < reg._typeIndex.size(); type_index++) {
        if (reg._typeIndex[type_index] == std::type_index(typeid(component)))
            break;
    }

    if (type_index == reg._typeIndex.size() && packet_type) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return {};
    } else {
        std::array<char, 37> uuid = generate_uuid();
        std::cout << "UUID CLIENT du packet: " << uuid.data() << std::endl;
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid};
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

void UDPServer::handle_receive_client(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "BYTES RECU DU SERVEUR: " << bytes_transferred << std::endl;
        Packet packet;
        std::vector<uint8_t> receivedComponent = unpack(packet, _recv_buffer, bytes_transferred);
        packet.display_packet();
        if (receivedComponent.size() == 0) {
            start_receive_client();
            return;
        }
        if (packet.packet_type == NEW_CONNECTION) {
            std::cout << "----------------------------NEW CONNECTION VOICI MON ID :----------------------" << packet.entity_id << std::endl;
            _entity_id = packet.entity_id;
            reg._player = packet.entity_id;
        } else if (packet.timestamp >= _last_timestamp) {
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
        start_receive_client();
    }
}

void UDPServer::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        Packet receivedPacket;
        std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);
        std::cout << "received packet in UDP" << std::endl;
        if (receivedComponent.size() == 0) {
            start_receive();
            return;
        }
        if (receivedPacket.packet_type == NEW_CONNECTION) {
            _clientsUDP[remote_endpoint_.port()] = remote_endpoint_;
            std::cout << "New player in UDP connected from " << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;
            receivedPacket.entity_id = _clientsUDP.size() - 1;
            send(receivedComponent, receivedPacket, remote_endpoint_);
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

template <typename T>
void UDPServer::send_client(const T &component, uint32_t entity_id, PacketType packet_type)
{
    std::vector<uint8_t> data = pack_client(component, entity_id, packet_type);

    if (data.size() == 0)
        return;
    try {
        std::cout << "Message sent to server UDP: " << " on adress " << _endpointServer.address() << " on port " << _endpointServer.port() << std::endl;
        socket_.send_to(asio::buffer(data), _endpointServer);
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void UDPServer::send(std::vector<uint8_t> message, asio::ip::udp::endpoint endpoint)
{
    try {
        socket_.send_to(asio::buffer(message), endpoint);
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void UDPServer::send(std::vector<uint8_t> component, Packet packet, asio::ip::udp::endpoint endpoint)
{
    std::vector<uint8_t> data;
    std::vector<uint8_t> packetBytes(reinterpret_cast<const uint8_t*>(&packet),
                                     reinterpret_cast<const uint8_t*>(&packet) + sizeof(Packet));
    data.insert(data.end(), packetBytes.begin(), packetBytes.end());
    data.insert(data.end(), component.begin(), component.end());

    if (data.empty())
        return;
    try {
        socket_.send_to(asio::buffer(data), endpoint);
        if (packet.packet_type == DATA_PACKET || packet.packet_type == NEW_CONNECTION) {
            data[4] = RESPONSE_PACKET;
            mtxSendPacket.lock();
            _queueSendPacket.push_back(std::make_pair(endpoint, data));
            mtxSendPacket.unlock();
        }
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

void UDPServer::saveData()
{
    for (int i = 0; i < _queue.size(); i++) {
        Packet packet = _queue[i].first;
        int size = _queue[i].second.size();
        char packet2[64];
        std::memcpy(packet2, _queue[i].second.data(), size);
        reg.registerPacket(packet.type_index, packet.entity_id, packet2);
        sendToAll(_queue[i].first, _queue[i].second, DATA_PACKET);
    }
    _queue.clear();
}

void UDPServer::saveData_client()
{
    std::cout << "GET DATA CLIENT" << std::endl;
    for (int i = 0; i < _queue.size(); i++) {
        Packet packet = _queue[i].first;
        int size = _queue[i].second.size();
        char packet2[64]= {0};
        std::memcpy(packet2, _queue[i].second.data(), size);
        reg.registerPacket(packet.type_index, packet.entity_id, packet2);
    }
}

void UDPServer::sendToAll(const Packet &packet, std::vector<uint8_t> component, PacketType packet_type)
{
    std::vector<uint8_t> packetBytes(reinterpret_cast<const uint8_t*>(&packet),
                                     reinterpret_cast<const uint8_t*>(&packet) + sizeof(Packet));
    std::vector<uint8_t> data;
    data.insert(data.end(), packetBytes.begin(), packetBytes.end());
    data.insert(data.end(), component.begin(), component.end());

    if (data.size() == 0) {
        return;
    }
    try {
        for (const auto &client : _clientsUDP) {
            std::cout << "Message sent to client UDP DANS SEND TO ALL: " << client.first << std::endl;
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
