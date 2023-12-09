#include "tcp.hpp"

TCPClient::TCPClient(std::size_t port) : _port(port), _ioContext(), _socket(_ioContext), _resolver(_ioContext), _endpoint(asio::ip::make_address("127.0.0.1"), this->_port), buffer()
{
    this->createClient();
    this->startAsyncOperations();
}

void TCPClient::createClient()
{
    try {
        asio::connect(this->_socket, this->_resolver.resolve("127.0.0.1", std::to_string(this->_port)));
        std::cout << "Connected to " << this->_endpoint << " successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
    }
}

void TCPClient::handleSend(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error) {
        std::cout << "Message sent" << std::endl;
    } else {
        std::cerr << "Error sending message: " << error.message() << std::endl;
    }
}

void TCPClient::sendToServer()
{
    std::string message("First co");
    asio::async_write(this->_socket, asio::buffer(message + "\n"), std::bind(&TCPClient::handleSend, this, std::placeholders::_1, std::placeholders::_2));
}

void TCPClient::handleReceive()
{
    std::cout << "Waiting for message..." << std::endl;
    asio::async_read_until(this->_socket, this->buffer, "\n", [this](const asio::error_code &error, std::size_t bytes_transferred) {
        std::cout << "Message received" << std::endl;
        if (!error) {
            std::istream input_stream(&this->buffer);
            std::string data;
            std::getline(input_stream, data);
            std::cout << "Received message: " << data << std::endl;
            if (data == "Welcome") {
                this->sendToServer();
                // exit(84);
            }
            if (data == "New client connected") {
                    std::string message("DEUXIMEME MESGGGA");
    // std::getline(std::cin, message);
                asio::async_write(this->_socket, asio::buffer(message + "\n"), std::bind(&TCPClient::handleSend, this, std::placeholders::_1, std::placeholders::_2));
            }
            handleReceive();
        } else {
            std::cerr << "Error reading from server: " << error.message() << std::endl;
        }
    });
}

void TCPClient::startAsyncOperations()
{
    while (true) {
        this->handleReceive();
        _ioContext.run();
    }
}
