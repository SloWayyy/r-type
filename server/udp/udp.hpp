/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** udp
*/

#ifndef UDP_HPP_
#define UDP_HPP_

    #include <asio.hpp>
    #include <iostream>
    #include <array>
    #include <functional>
    #include <memory>

// std::string make_daytime_string()
// {
//   using namespace std; // For time_t, time and ctime;
//   time_t now = time(0);
//   return ctime(&now);
// }

class Udp_server {
    public:
        Udp_server(asio::io_context& io_context, std::size_t port);
    private:
        void start_receive();
        void handle_receive(const asio::error_code& error, std::size_t bytes_transferred);
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred);

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
};

#endif /* !UDP_HPP_ */
