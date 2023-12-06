/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <asio.hpp>
#include <iostream>

int main(int ac, char **av) {
    try {
        asio::io_context io_context;
        asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));

        asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), std::atoi(av[1]));

        std::string message = "Hello, server!";

        socket.send_to(asio::buffer(message), server_endpoint);

        io_context.run();
        std::cout << "Message sent" << std::endl;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}