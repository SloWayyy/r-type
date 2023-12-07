/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <asio.hpp>
#include <iostream>
#include "../server/udp/udp.hpp"

void handle_receive(const asio::error_code &error, std::size_t bytes_transferred, std::array<char, 1024> recv_buffer)
{
    if (!error)
    {
        std::string received_message(recv_buffer.data(), bytes_transferred);
        std::cout << "Message reçu : " << received_message << std::endl;
    }
}

int main(int ac, char **av)
{
    UDPSERVER server(std::atoi(av[1]));
    int i = 0;
    try
    {
        
        std::array<char, 1024> recv_buffer;
        if (i == 0)
        {
            asio::io_context io_context;
            asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));

            asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 5555);

            std::string message = "Hello, server!";
            std::cout << socket.local_endpoint().port() << std::endl;
            socket.send_to(asio::buffer(message), server_endpoint);

            socket.async_receive_from(
                asio::buffer(recv_buffer), server_endpoint,
                std::bind(&handle_receive,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          recv_buffer));
            io_context.run();
            i++;
            std::cout << "Message sent" << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}