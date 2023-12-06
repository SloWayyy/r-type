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
        void start_receive() {
            socket_.async_receive_from(
                asio::buffer(recv_buffer_), remote_endpoint_,
                std::bind(&Udp_server::handle_receive, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }

        void handle_receive(const asio::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::string received_message(recv_buffer_.data(), bytes_transferred);
                std::cout << "Message reçu : " << received_message << std::endl;

                // pour avoir l'adresse du client et lui renvoyer un message
                // std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
                // socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                //     std::bind(&udp_server::handle_send, this, message,
                //     std::placeholders::_1,
                //     std::placeholders::_2));
                // std::cout << "Message recu" << message << std::endl;
                start_receive();

            }
        }
        void handle_send(std::shared_ptr<std::string> message, const asio::error_code& error, std::size_t bytes_transferred) {
        }

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
};

#endif /* !UDP_HPP_ */
