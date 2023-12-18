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
    start_receive();
    send(" ", 2, NEW_CONNECTION);
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

// std::unordered_map<uint32_t, std::type_index> _typeIndex = {
//     {2, typeid(Position)}
// };

std::string UDPClient::unpack(Packet &packet)
{
    try {
        std::memcpy(&packet, _recv_buffer.data(), sizeof(Packet));
        std::string component(_recv_buffer.data() + sizeof(Packet), _recv_buffer.data() + sizeof(Packet) + sizeof(component));
        return component;
    } catch (const std::exception &e) {
        std::cerr << "ERROR unpack: " << e.what() << std::endl;
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
        std::string receivedComponent = unpack(packet);
        if (receivedComponent.empty()) {
            std::cout << "ERROR: component empty" << std::endl;
            start_receive();
            return;
        }
        // rendre generique
        //
        if (packet.timestamp >= _last_timestamp) {
            std::cout << packet.uuid.data() << std::endl;
            // send(pos, packet.entity_id, RESPONSE_PACKET);
            _last_timestamp = packet.timestamp;
            mtx.lock();
            _queue.push_back(std::make_pair(packet, receivedComponent));
            mtx.unlock();
        } else {
            std::cout << "je ne traite pas l'information mais j envoi qd meme au serv" << std::endl;
            // send(receivedComponent, packet.entity_id, RESPONSE_PACKET);
        }
        start_receive();
    }
}

template <typename T>
std::string UDPClient::pack(T const& component, uint32_t entity_id, PacketType packet_type)
{
    uint32_t type_index = 0;

    for (; type_index < reg._typeIndex.size(); type_index++)
        if (reg._typeIndex[type_index] == std::type_index(typeid(component)))
            break;

    if ((type_index) == -1) {
        std::cerr << "ERROR: type_index not found message not send" << std::endl;
        return "";
    } else {
        std::array<char, 37> uuid = generate_uuid();
        std::cout << "UUID CLIENT du packet: " << uuid.data() << std::endl;
        Packet packet = {_magic_number, packet_type, std::time(nullptr), entity_id, type_index, uuid};
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

void UDPClient::run()
{
    std::cout << "Server is running on port " << this->_port << std::endl;
    _io_context.run();
}

template <typename T>
void UDPClient::send(const T &component, uint32_t entity_id, PacketType packet_type)
{
    std::string data = pack(component, entity_id, packet_type);
    if (data.empty()) {
        std::cout << "ERROR: data empty" << std::endl;
        return;
    }
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
