/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** networkServer.hpp
*/

#ifndef NETWORKSERVER_HPP
#define NETWORKSERVER_HPP

#include <exception>
#include <arpa/inet.h>
#include <poll.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "logger.hpp"

#define MAX_CLIENTS 4096

class NetworkServer {
    public:
        class NetworkServerException final : public std::exception {
            public:
            explicit NetworkServerException(const std::string &message) : _message(message) {}
            const char *what() const noexcept override { return _message.c_str(); }
            private:
            std::string _message;
        };

        explicit NetworkServer(int port, Logger &logger);
        ~NetworkServer();
        void run();


    private:
        static int createSocket();
        sockaddr_in createServerAddress() const;
        void bindSocket() const;
        void startListening() const;
        void createServerSocketPollEntry();
        static std::string getClientIp(int socket_fd);

        Logger &_logger;
        int _serverPort;
        int _serverSocket;
        sockaddr_in _serverAddress;
        pollfd _fds[MAX_CLIENTS + 1];
        int _nbSockets;
};

#endif //NETWORKSERVER_HPP
