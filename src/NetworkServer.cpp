/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** networkServer.cpp
*/

#include "NetworkServer.hpp"

#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <format>
#include "Logger.hpp"

namespace network
{
    NetworkServer::NetworkServer(const int port): _port(port)
    {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (_serverSocket < 0) {
            throw std::runtime_error(std::strerror(errno));
        }

        _serverAddress.sin_family = AF_INET;
        _serverAddress.sin_port = htons(port);
        _serverAddress.sin_addr.s_addr = INADDR_ANY;

        if (bind(_serverSocket, (sockaddr *)&_serverAddress, sizeof(_serverAddress)) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }

        if (listen(_serverSocket, 5) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }

        pollfd serverFdEntry;
        serverFdEntry.fd = _serverSocket;
        serverFdEntry.events = POLLIN;
        serverFdEntry.revents = 0;

        _poll_fds.push_back(serverFdEntry);
    }

    NetworkServer::~NetworkServer()
    {
        for (auto & pfd: _poll_fds) {
            close(pfd.fd);
        }
    }

    std::string NetworkServer::remoteIpStringFromFd(const int fd)
    {
        sockaddr_in addr {};
        socklen_t addr_size = sizeof(struct sockaddr_in);
        std::string ip;

        if (getpeername(fd, (sockaddr *)&addr, &addr_size)) {
            char clientIp[INET_ADDRSTRLEN];
            return inet_ntop(AF_INET, &addr.sin_addr, clientIp, INET_ADDRSTRLEN);
        }
        return "unknown";
    }

    std::string NetworkServer::remotePortFromFd(const int fd)
    {
        sockaddr_in addr {};
        socklen_t addr_size = sizeof(struct sockaddr_in);
        std::string disconnMsg;
        if (getpeername(fd, (sockaddr *)&addr, &addr_size)) {
            return std::format("{}", ntohs(addr.sin_port));
        }
        return "unknown";
    }

    void NetworkServer::handleNewConnection()
    {
        sockaddr_in client {};
        socklen_t clientLen = sizeof(client);

        Logger::log(Logger::LogLevel::TRACE, "incoming connection...");

        int clientSocketFd = accept(_serverSocket, (sockaddr *)&client, &clientLen);

        if (clientSocketFd < 0) {
            std::cerr << ERROR_LOG_TITLE << CANNOT_INIT_CONN_ERROR << std::endl;
            return;
        }

        pollfd clientFdEntry;
        clientFdEntry.fd = clientSocketFd;
        clientFdEntry.events = POLLIN;
        clientFdEntry.revents = 0;
        _poll_fds.push_back(clientFdEntry);

        // logging new connection
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, clientIp, INET_ADDRSTRLEN);
        uint16_t clientPort = ntohs(client.sin_port);
        std::string newConMsg = std::format("new connection initialized from {} on remote port {}", clientIp, clientPort);
        Logger::log(Logger::LogLevel::INFO, newConMsg);
    }

    void NetworkServer::handleDisconnect(const int fd)
    {
        if (fd < 0) {
            std::cerr << ERROR_LOG_TITLE << INVALID_SOCKET_DCONN_ERR << std::endl;
            return;
        }

        for (size_t i = 0; i < _poll_fds.size(); i++) {
            if (_poll_fds[i].fd == fd) {
                // saving remote client data for logging
                std::string disconnMsg = std::format(
                    "disconnected from {} on port {}",
                    remoteIpStringFromFd(fd),
                    remotePortFromFd(fd)
                    );

                // closing server-client connection
                close(fd);
                _poll_fds.erase(_poll_fds.begin() + i);

                // logging disconnected client
                Logger::log(Logger::LogLevel::INFO, disconnMsg);
            }
        }
    }

    void NetworkServer::handleRequest(const int fd)
    {
        char buf[1024] = {0};
        ssize_t ret;

        std::string request;

        do {
            ret = read(fd, buf, sizeof(buf));
            request += buf;
        } while (ret > 0);

        Logger::log(Logger::LogLevel::TRACE, std::format(
            "received request from {} on port {}: \"{}\"",
            remoteIpStringFromFd(fd),
            remotePortFromFd(fd),
            request)
            );

        if (ret == 0) {
            handleDisconnect(fd);
            return;
        }

        // do whatever the fuck we are supposed to do with the request
    }

    void NetworkServer::handleConnections()
    {
        while (true) {
            int pollResult = poll(_poll_fds.data(), _poll_fds.size(), -1);

            if (pollResult < 0) {
                std::cerr << ERROR_LOG_TITLE << std::strerror(errno) << std::endl;
                continue;
            }

            for (auto & pfd: _poll_fds) {
                if (pfd.revents & POLLIN) {

                    if (pfd.fd == _serverSocket) {
                        handleNewConnection();
                        continue;
                    }
                    handleRequest(pfd.fd);
                }
                if (pfd.revents & POLLHUP) {
                    handleDisconnect(pfd.fd);
                }
            }
        }
    }
}
