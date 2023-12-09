#ifndef TCP_CLIENT_HPP_
#define TCP_CLIENT_HPP_

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <functional>

class TCPClient {
public:
    TCPClient(std::size_t port);

private:
    void createClient();
    void handleSend(const asio::error_code &error, std::size_t bytes_transferred);
    void sendToServer();
    void handleReceive();
    void startAsyncOperations();

    std::size_t _port;
    asio::io_context _ioContext;
    asio::ip::tcp::socket _socket;
    asio::ip::tcp::resolver _resolver;
    asio::ip::tcp::endpoint _endpoint;
    asio::streambuf buffer;
};

#endif /* TCP_CLIENT_HPP_ */
