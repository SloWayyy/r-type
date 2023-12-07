/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#include "udp.hpp"

UDPSERVER::UDPSERVER(std::size_t port)
    : socket_(_io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    std::cout << "Constructing UDPSERVER..." << port << std::endl;
    start_receive();
    send("test");
    _io_context.run();
    std::cout << "AFTER RUN" << port << std::endl;
}

void UDPSERVER::start_receive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UDPSERVER::handle_receive, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
    // write message to client
    // std::string message = "Hello, client!";
    // response(message);
}

void UDPSERVER::handle_receive(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::string received_message(recv_buffer_.data(), bytes_transferred);
        std::cout << "Message reçu : " << received_message << std::endl;
        if (received_message == "test") {
            std::string message = "Hello, client!";
            response(message);
        }

        // pour avoir l'adresse du client et lui renvoyer un message
        // std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
        // socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
        //     std::bind(&UDPSERVER::handle_send, this, message,
        //     std::placeholders::_1,
        //     std::placeholders::_2));
        // std::cout << "Message recu" << message << std::endl;
        start_receive();
    }
}

void UDPSERVER::response(std::string message)
{
    std::cout << "RESPONSE" << std::endl;
    std::shared_ptr<std::string> message2(new std::string(message));
    std::cout << remote_endpoint_.address() << " " << remote_endpoint_.port() << std::endl;
    socket_.async_send_to(asio::buffer(*message2), remote_endpoint_,
        std::bind(&UDPSERVER::handle_send, this, message2,
        std::placeholders::_1,
        std::placeholders::_2));
    std::cout << "Message recu" << message << std::endl;
}

// asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));


// std::cout << socket.local_endpoint().port() << std::endl;

// socket.async_receive_from(
//     asio::buffer(recv_buffer), server_endpoint,
//     std::bind(&handle_receive,
//                 std::placeholders::_1,
//                 std::placeholders::_2,
//                 recv_buffer));

void UDPSERVER::send(std::string message)
{
    asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 5555);
    std::string message1 = "Hello, server!";
    socket_.send_to(asio::buffer(message), server_endpoint);

}

void UDPSERVER::handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred)
{
    std::cout << "Message envoyé" << std::endl;
}
