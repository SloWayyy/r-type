/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** socket
*/

#ifndef TCP_SERVER_HPP_
    #define TCP_SERVER_HPP_

    #include <asio.hpp>
    #include <iostream>
    #include <unordered_map>
    #include <functional>
    #include <memory>

    #define PLAYER_NOT_FOUND 15

class TCPServer {
    public:
        TCPServer(std::size_t port, std::size_t portUDP, std::string ip);
        ~TCPServer();
        int createSocket();
        void run();
        void startAccept();
        void handleRead(std::shared_ptr<asio::ip::tcp::socket> client);
        void sendMessageToAllClients(const std::string &message);
        void sendMessageToAClient(const std::string &message, std::shared_ptr<asio::ip::tcp::socket> client);
        void closeConnection(std::shared_ptr<asio::ip::tcp::socket> client);
        size_t getClientByIndex(int index) {
            int i = 0;
        
            for (auto &client : _clientsInfo) {
                if (i == index)
                    return client.first;
                i++;
            }
            return PLAYER_NOT_FOUND;
        }
        std::unordered_map<size_t, std::shared_ptr<asio::ip::tcp::socket>> _clientsInfo;
        std::vector<std::string> _ClientMessages;
        std::thread _thread;
        std::mutex _mtx;

    private:
        std::size_t _port;
        std::size_t _portUDP;
        asio::io_context _ioContext;
        asio::streambuf buffer;
        std::string _ip;
        asio::ip::tcp::endpoint _endpoint;
        asio::ip::tcp::acceptor _acceptor;
        asio::error_code _ec;
    protected:
};

#endif /* !TCP_SERVER_HPP_ */
