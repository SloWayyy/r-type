/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include <random>
#include <typeindex>
#include <unordered_map>

Udp::Udp(std::size_t port, std::string ip, registry& reg, UpdateGame& updateGame)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), 0))
    , _magic_number(4242)
    , reg(reg)
    , updateGame(updateGame)
{
    try {
        this->socket_ = asio::ip::udp::socket(_io_context, asio::ip::udp::endpoint(asio::ip::make_address(ip), port));
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP binding socket: " << ec.what() << std::endl;
        std::exit(84);
    }

    this->_port = socket_.local_endpoint().port();
    _thread = std::thread(&Udp::run, this);
    start_receive();
}

Udp::Udp(std::string ip, registry& reg)
    : _endpointServer(asio::ip::make_address(ip), 4242)
    , socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0))
    , _last_timestamp(0)
    , reg(reg)
    , updateGame(updateGame)
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

void Udp::start_receive(bool client)
{
    try {
        if (client) {
            socket_.async_receive_from(asio::buffer(_recv_buffer), remote_endpoint_,
                std::bind(&Udp::handleReceiveClient, this, std::placeholders::_1, std::placeholders::_2));
        } else {
            socket_.async_receive_from(asio::buffer(_recv_buffer), remote_endpoint_,
                std::bind(&Udp::handleReceiveServer, this, std::placeholders::_1, std::placeholders::_2));
        }
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP binding socket: " << ec.what() << std::endl;
        return;
    }
}

std::vector<uint8_t> Udp::createPacket(std::vector<uint8_t> component, Packet packet)
{
    std::vector<uint8_t> data;
    std::vector<uint8_t> packetBytes(reinterpret_cast<const uint8_t*>(&packet),
        reinterpret_cast<const uint8_t*>(&packet) + sizeof(Packet));
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
    Packet packet = { _magic_number, packet_type, std::time(nullptr), entity_id, 0, generate_uuid() };
    std::vector<uint8_t> result;

    result.resize(sizeof(Packet));
    const uint8_t* packetBytes = reinterpret_cast<const uint8_t*>(&packet);
    std::copy(packetBytes, packetBytes + sizeof(Packet), result.begin());
    return result;
}

// Basic packet
template <typename T> std::vector<uint8_t> Udp::createPacket(PacketType packet_type, T const& component, uint32_t entity_id)
{
    uint32_t type_index = reg.findTypeIndex(component).value_or(reg._typeIndex.size());

    if (type_index == reg._typeIndex.size()) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return {};
    } else {
        std::array<char, 37> uuid = generate_uuid();
        Packet packet = { _magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid };
        try {
            std::vector<uint8_t> result;
            result.resize(sizeof(Packet) + sizeof(T));
            const uint8_t* packetBytes = reinterpret_cast<const uint8_t*>(&packet);
            std::copy(packetBytes, packetBytes + sizeof(Packet), result.begin());
            const uint8_t* componentBytes = reinterpret_cast<const uint8_t*>(&component);
            std::copy(componentBytes, componentBytes + sizeof(T), result.begin() + sizeof(Packet));
            return result;
        } catch (const std::exception& e) {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return {};
        }
    }
}

std::vector<uint8_t> Udp::unpack(Packet& packet, std::array<uint8_t, 1024> query, std::size_t bytes_transferred)
{
    try {
        // Decryptage en XOR
        for (std::size_t i = 0; i < bytes_transferred; i++)
            query[i] ^= CRYPT_KEY;

        std::memcpy(&packet, query.data(), sizeof(Packet));
        std::vector<uint8_t> component(query.begin() + sizeof(Packet), query.begin() + bytes_transferred);
        return component;
    } catch (const std::exception& e) {
        std::cerr << "ERROR unpack: " << e.what() << std::endl;
        return {};
    }
}

int Udp::handleErrorReceive(const asio::error_code& error, std::vector<uint8_t> receivedComponent, Packet receivedPacket, bool isClient)
{
    if (error)
        return -1;
    if (receivedComponent.size() == 0 && receivedPacket.packet_type != NEW_CONNECTION && receivedPacket.packet_type != RESPONSE_PACKET) {
        start_receive(isClient);
        return -1;
    }
    return 0;
}

void Udp::handleReceiveClient(const asio::error_code& error, std::size_t bytes_transferred)
{
    Packet receivedPacket;
    std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);

    if (handleErrorReceive(error, receivedComponent, receivedPacket, true) == -1)
        return;

    if (receivedPacket.packet_type == NEW_CONNECTION) {
        handleNewConnection(receivedPacket, receivedComponent);
    } else if (receivedPacket.timestamp >= _last_timestamp) {
        handleTimestampUpdate(receivedPacket, receivedComponent);
    } else {
        std::cout << "OUTDATED PACKET" << std::endl;
    }
    receivedPacket.packet_type = RESPONSE_PACKET;
    sendClientToServer(receivedPacket);
    start_receive(true);
}

void Udp::handleNewConnection(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent)
{
    if (_entity_id == -1) {
        _entity_id = receivedPacket.entity_id;
        reg._player = receivedPacket.entity_id;
        mtx.lock();
        _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
        mtx.unlock();
    } else {
        mtx.lock();
        _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
        mtx.unlock();
    }
}

void Udp::handleTimestampUpdate(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent)
{
    _last_timestamp = receivedPacket.timestamp;
    mtx.lock();
    _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
    mtx.unlock();
}

void Udp::sendPlayerListToClient(std::vector<std::vector<uint8_t>> entities, Packet receivedPacket)
{
    for (size_t i = 1; i < entities.size(); i += 1) {
        receivedPacket.type_index = i - 1;
        std::memcpy(&receivedPacket.entity_id, entities[0].data(), sizeof(uint32_t));
        sendToAll(DATA_PACKET, entities[i], receivedPacket);
    }
    for (const auto& _entity : _sparseArray) {
        for (size_t i = 1; i < _entity.size(); i += 1) {
            receivedPacket.type_index = i - 1;
            std::memcpy(&receivedPacket.entity_id, _entity[0].data(), sizeof(uint32_t));
            sendServerToClient(NEW_CONNECTION, _entity[i], receivedPacket);
        }
    }
}

void Udp::handleReceiveServer(const asio::error_code& error, std::size_t bytes_transferred)
{
    Packet receivedPacket;
    std::vector<uint8_t> receivedComponent = unpack(receivedPacket, _recv_buffer, bytes_transferred);

    if (handleErrorReceive(error, receivedComponent, receivedPacket, false) == -1)
        return;
    processReceivedPacket(receivedPacket, receivedComponent);
}

void Udp::processReceivedPacket(const Packet& receivedPacket, const std::vector<uint8_t>& receivedComponent)
{
    const std::map<std::size_t, std::function<void(const Packet&, const std::vector<uint8_t>&)>> ptr_fct
        = { { NEW_CONNECTION, [this](const Packet& packet, const std::vector<uint8_t>& component) { handleNewConnection(packet); } },
              { RESPONSE_PACKET, [this](const Packet& packet, const std::vector<uint8_t>& component) { handleResponsePacket(packet); } } };
    auto it = ptr_fct.find(receivedPacket.packet_type);

    if (it != ptr_fct.end()) {
        it->second(receivedPacket, receivedComponent);
    } else if (receivedPacket.magic_number != _magic_number) {
        std::cerr << "ERROR: magic number not valid in received packet" << std::endl;
        start_receive();
    } else {
        mtxQueue.lock();
        _queue.push_back(std::make_pair(receivedPacket, receivedComponent));
        mtxQueue.unlock();
        start_receive();
    }
}

void Udp::handleNewConnection(const Packet& receivedPacket)
{
    _clientsUDP[remote_endpoint_.port()] = remote_endpoint_;
    std::vector<std::vector<uint8_t>> entities = updateGame.updateEntity();
    _sparseArray.push_back(entities);
    sendPlayerListToClient(entities, receivedPacket);
    start_receive();
}

void Udp::handleResponsePacket(const Packet& receivedPacket)
{
    mtxSendPacket.lock();
    std::size_t size = _queueSendPacket.size();
    for (std::size_t i = 0; i < size;) {
        Packet queryPacket;
        std::memcpy(&queryPacket, _queueSendPacket[i].second.data(), sizeof(Packet));
        if (receivedPacket.uuid == queryPacket.uuid && remote_endpoint_ == _queueSendPacket[i].first) {
            _queueSendPacket.erase(_queueSendPacket.begin() + i);
            size--;
        } else {
            i++;
        }
    }
    mtxSendPacket.unlock();
    start_receive();
}

std::array<char, 37> Udp::generate_uuid()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::array<char, 37> uuid = { 0 };
    std::string hexChars = "0123456789abcdef";

    for (int i = 0; i < 37; i++)
        uuid[i] = hexChars[dis(gen)];
    return uuid;
}

void Udp::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    this->_io_context.run();
    std::cout << "Server is closing" << std::endl;
}

template <typename... Args> void Udp::sendClientToServer(Args... args)
{
    auto data = createPacket(args...);

    // Encryptage en XOR
    std::vector<uint8_t> cryptData;
    for (std::size_t i = 0; i < data.size(); i++)
        cryptData.push_back(data[i] ^ CRYPT_KEY);

    if (data.size() == 0)
        return;
    try {
        // std::cout << "Sent to server UDP: type(" << data[4] << ") on adress " << _endpointServer.address() << " on port " << _endpointServer.port()
        //           << std::endl;
        socket_.send_to(asio::buffer(cryptData), _endpointServer);
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

template <typename... Args> void Udp::sendServerToClient(PacketType packet_type, Args... args)
{
    std::vector<uint8_t> data = createPacket(args...);

    // Encryptage en XOR
    std::vector<uint8_t> cryptData = cryptMessage(data);

    if (data.empty())
        return;
    try {
        socket_.send_to(asio::buffer(cryptData), remote_endpoint_);
        if (packet_type == DATA_PACKET) {
            mtxSendPacket.lock();
            _queueSendPacket.push_back(std::make_pair(remote_endpoint_, data));
            mtxSendPacket.unlock();
        }
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void Udp::sendServerToAClient(std::vector<uint8_t> data, asio::ip::udp::endpoint endpoint)
{
    // Encryptage en XOR
    std::vector<uint8_t> cryptData = cryptMessage(data);
    std::cout << "data :" << data.data() << " cryptData :" << cryptData.data() << std::endl;

    try {
        socket_.send_to(asio::buffer(cryptData), endpoint);
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

template <typename... Args> void Udp::sendToAll(PacketType packet_type, Args... args)
{
    std::vector<uint8_t> data = createPacket(args...);

    // Encryptage en XOR
    std::vector<uint8_t> cryptData = cryptMessage(data);

    if (data.size() == 0)
        return;
    try {
        for (const auto& client : _clientsUDP) {
            socket_.send_to(asio::buffer(cryptData), client.second);
            if (packet_type == DATA_PACKET) {
                mtxSendPacket.lock();
                _queueSendPacket.push_back(std::make_pair(client.second, data));
                mtxSendPacket.unlock();
            }
        }
    } catch (const asio::system_error& ec) {
        std::cerr << "ERROR UDP sending message" << ec.what() << std::endl;
    }
}

void Udp::updateSparseArray(bool isClient)
{
    for (auto& i : _queue) {
        Packet header = i.first;
        auto data = i.second;
        char component[64] = { 0 };
        std::memcpy(component, data.data(), data.size());
        reg.registerPacket(header.type_index, header.entity_id, component);
        if (!isClient)
            sendToAll(DATA_PACKET, data, header);
    }
    _queue.clear();
}

// This function is not used in
// template <typename... Args> void Udp::sendClientToServer(Args... args)
// because of the return value is not the same always
// so we do the encryption in the function directly
std::vector<uint8_t> Udp::cryptMessage(std::vector<uint8_t> message)
{
    std::vector<uint8_t> cryptMessage;

    for (std::size_t i = 0; i < message.size(); i++)
        cryptMessage.push_back(message[i] ^ CRYPT_KEY);
    return cryptMessage;
}
