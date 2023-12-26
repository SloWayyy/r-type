/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include <random>
#include <typeindex>
#include <unordered_map>

Udp::Udp(std::size_t port, std::string ip, registry &reg) : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), 0)), _magic_number(4242), reg(reg)
{
    try {
        this->socket_ = asio::ip::udp::socket(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port));
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP binding socket: " << ec.what() << std::endl;
        std::exit(84);
    }

    this->_port = socket_.local_endpoint().port();
    _thread = std::thread(&Udp::run, this);
    start_receive();
}

Udp::Udp(std::string ip, registry &reg) : _endpointServer(asio::ip::make_address(ip), 4242), socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)), _last_timestamp(0), reg(reg)
{
    _thread = std::thread(&Udp::run, this);
    _port = socket_.local_endpoint().port();
    start_receive(true);
    sendClientToServer(NEW_CONNECTION, 0);
}

Udp::~Udp()
{
    std::cout << "Server is closing" << std::endl;
    _thread.join();
}

bool operator==(const Packet &lhs, const Packet &rhs)
{
    return lhs.magic_number == rhs.magic_number && lhs.packet_type == rhs.packet_type && lhs.timestamp == rhs.timestamp && lhs.entity_id == rhs.entity_id && lhs.type_index == rhs.type_index;
}

void Udp::start_receive(bool client)
{
    if (client) {
        socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&Udp::handleReceiveClient, this,
        std::placeholders::_1,
        std::placeholders::_2));
    } else {
        socket_.async_receive_from(
        asio::buffer(_recv_buffer), remote_endpoint_,
        std::bind(&Udp::handleReceiveServer, this,
        std::placeholders::_1,
        std::placeholders::_2));
    }
}

std::vector<uint8_t> Udp::createPacket(std::vector<uint8_t> component, Packet packet)
{
    std::vector<uint8_t> data;
    std::vector<uint8_t> packetBytes(reinterpret_cast<const uint8_t *>(&packet),

    reinterpret_cast<const uint8_t *>(&packet) + sizeof(Packet));
    data.insert(data.end(), packetBytes.begin(), packetBytes.end());
    data.insert(data.end(), component.begin(), component.end());
    return data;
}

std::array<uint8_t, sizeof(Packet)> Udp::createPacket(Packet packet)
{
    std::array<uint8_t, sizeof(Packet)> buffer;
    std::memcpy(buffer.data(), &packet, sizeof(Packet));
    return buffer;
}

// Packet for new connection
std::vector<uint8_t> Udp::createPacket(PacketType packet_type, uint32_t entity_id)
{
    Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, 0, generate_uuid()};
    std::vector<uint8_t> result;

    result.resize(sizeof(Packet));
    const uint8_t *packetBytes = reinterpret_cast<const uint8_t *>(&packet);
    std::copy(packetBytes, packetBytes + sizeof(Packet), result.begin());
    return result;
}

// Basic packet
template <typename T>
std::vector<uint8_t> Udp::createPacket(PacketType packet_type, T const &component, uint32_t entity_id)
{
    size_t type_index = reg.findTypeIndex(component).value_or(reg._typeIndex.size());

    if (type_index == reg._typeIndex.size()) {
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

std::vector<uint8_t> Udp::unpack(Packet &packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred)
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

int Udp::handleErrorReceive(const asio::error_code &error, std::vector<uint8_t> receivedComponent, Packet receivedPacket, bool isClient)
{
    if (error)
        return -1;
    if (receivedComponent.size() == 0 && receivedPacket.packet_type != NEW_CONNECTION) {
        start_receive(isClient);
        return -1;
    }
    return 0;
}

void Udp::handleReceiveClient(const asio::error_code &error, std::size_t bytes_transferred)
{
    Packet receivedPacket;
    std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);
    std::cout << "size " << receivedComponent.size() << std::endl;

    if (handleErrorReceive(error, receivedComponent, receivedPacket, true) == -1)
        return;
    if (receivedPacket.packet_type == NEW_CONNECTION) {
        Position pos_player;
        std::memcpy(&pos_player, receivedComponent.data(), sizeof(Position));
        std::cout << "pos_player: " << pos_player.x << pos_player.y << std::endl;
        uint32_t player = reg.addEntity();
        std::cout << "NEW CONNECTION " << player << std::endl;
        auto &position = reg.getComponent<Position>();
        auto &velocity = reg.getComponent<Velocity>();
        auto &sprite = reg.getComponent<Sprite>();
        auto &size = reg.getComponent<Size>();
        
        sprite.emplace_at(player, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
        position.emplace_at(player, pos_player.x, pos_player.y);
        size.emplace_at(player, 1.5, 1.5);
        velocity.emplace_at(player, 0, 0, 0, 0, 0);
        _entity_id = player;
        reg._player = player;
    } else if (receivedPacket.packet_type == NEW_PLAYER) {
        Position pos_player;
        std::memcpy(&pos_player, receivedComponent.data(), sizeof(Position));
        std::cout << "pos_player: " << pos_player.x << pos_player.y << std::endl;
        uint32_t player = reg.addEntity();
        std::cout << "NEW PLAYER " << player << std::endl;
        auto &position = reg.getComponent<Position>();
        auto &velocity = reg.getComponent<Velocity>();
        auto &sprite = reg.getComponent<Sprite>();
        auto &size = reg.getComponent<Size>();
        
        sprite.emplace_at(player, "../game/assets/spaceShip.png", sf::IntRect(198, 0, 32, 32));
        position.emplace_at(player, pos_player.x, pos_player.y);
        size.emplace_at(player, 1.5, 1.5);
        velocity.emplace_at(player, 0, 0, 0, 0, 0);
    } else if (receivedPacket.timestamp >= _last_timestamp) {
        _last_timestamp = receivedPacket.timestamp;
        mtx.lock();
        _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
        mtx.unlock();
    } else
        std::cout << "OUTDATED PACKET" << std::endl;

    receivedPacket.packet_type = RESPONSE_PACKET;
    sendClientToServer(receivedPacket);
    start_receive(true);
}

int pos = 50;

void Udp::handleReceiveServer(const asio::error_code &error, std::size_t bytes_transferred)
{
    Packet receivedPacket;
    std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);

    if (handleErrorReceive(error, receivedComponent, receivedPacket, false) == -1)
        return;
    if (receivedPacket.packet_type == NEW_CONNECTION) {
        uint32_t player = reg.addEntity();
        std::cout << "NEW CONNECTION " << player << std::endl;
        auto &position = reg.getComponent<Position>();
        auto &velocity = reg.getComponent<Velocity>();
        auto &sprite = reg.getComponent<Sprite>();
        auto &size = reg.getComponent<Size>();

        sprite.emplace_at(player, "../game/assets/spaceShipBlue.png", sf::IntRect(198, 0, 32, 32));
        position.emplace_at(player, 50, pos);
        size.emplace_at(player, 1.5, 1.5);
        velocity.emplace_at(player, 0, 0, 0, 0, 0);
        _clientsUDP[remote_endpoint_.port()] = remote_endpoint_;
        receivedPacket.entity_id = player;
        Position pos_player = {50, pos};
        std::cout << "pos: " << pos << std::endl;
        pos += 50;
        sendServerToClient(NEW_CONNECTION, remote_endpoint_, NEW_CONNECTION , pos_player, player);
        for (const auto &client : _clientsUDP) {
            if (client.second.port() != remote_endpoint_.port())
                sendServerToClient(NEW_PLAYER, client.second, NEW_PLAYER, pos_player, player);
        }
        // sendToAll(NEW_CONNECTION, NEW_CONNECTION, pos_player, player);
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

std::array<char, 37> Udp::generate_uuid()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::array<char, 37> uuid = {0};
    std::string hexChars = "0123456789abcdef";

    for (int i = 0; i < 37; i++)
        uuid[i] = hexChars[dis(gen)];
    return uuid;
}

void Udp::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_io_context.run();
}

template <typename... Args>
void Udp::sendClientToServer(Args... args)
{
    auto data = createPacket(args...);

    if (data.size() == 0)
        return;
    try {
        std::cout << "Message sent to server UDP: " << " on adress " << _endpointServer.address() << " on port " << _endpointServer.port() << std::endl;
        socket_.send_to(asio::buffer(data), _endpointServer);
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

template <typename... Args>
void Udp::sendServerToClient(PacketType packet_type, asio::ip::udp::endpoint endpoint, Args... args)
{
    std::vector<uint8_t> data = createPacket(args...);

    if (data.empty())
        return;
    try {
        socket_.send_to(asio::buffer(data), endpoint);
        if (packet_type == DATA_PACKET || packet_type == NEW_CONNECTION) {
            data[4] = RESPONSE_PACKET;
            mtxSendPacket.lock();
            _queueSendPacket.push_back(std::make_pair(endpoint, data));
            mtxSendPacket.unlock();
        }
    } catch (const asio::system_error &ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

template <typename ...Args>
void Udp::sendToAll(PacketType packet_type, Args ...args)
{
    std::vector<uint8_t> data = createPacket(args...);

    if (data.size() == 0)
        return;
    try {
        for (const auto &client : _clientsUDP) {
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

void Udp::updateSparseArray(bool isClient)
{
    for (auto &i : _queue) {
        Packet header = i.first;
        auto data = i.second;
        char component[64] = {0};

        std::memcpy(component, data.data(), data.size());
        reg.registerPacket(header.type_index, header.entity_id, component);

        if (!isClient)
            sendToAll(DATA_PACKET, data, header);
    }

    if (!isClient)
        _queue.clear();
}

