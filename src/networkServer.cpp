/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** networkServer.cpp
*/

#include "networkServer.hpp"

int NetworkServer::createSocket()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;

    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt SO_REUSEADDR failed");
    }

    if (serverSocket == -1) {
        throw NetworkServerException("Socket creation failed");
    }
    return serverSocket;
}

sockaddr_in NetworkServer::createServerAddress() const
{
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(_serverPort);
    return serverAddress;
}

void NetworkServer::bindSocket() const
{
    const int res = bind(_serverSocket, (sockaddr *) &_serverAddress, sizeof(_serverAddress));
    if (res == -1) {
        close(_serverSocket);
        throw NetworkServerException(strerror(errno));
    }
}

void NetworkServer::startListening() const
{
    const int res = listen(_serverSocket, SOMAXCONN);
    if (res == -1) {
        close(_serverSocket);
        throw NetworkServerException("Cannot listen on socket");
    }
}

void NetworkServer::createServerSocketPollEntry()
{
    _fds[0].fd = _serverSocket;
    _fds[0].events = POLLIN;
    _nbSockets = 1;
}

NetworkServer::NetworkServer(const int port, Logger &logger) : _logger(logger), _serverPort(port),
                                                               _serverSocket(createSocket()),
                                                               _serverAddress(createServerAddress()),
                                                               _fds({}), _clients(std::unordered_map<int, Client>()), _nbSockets(0)
{
    bindSocket();
    startListening();
    createServerSocketPollEntry();
}

NetworkServer::~NetworkServer()
{
    // close every socket
    for (int i = 0; i < _nbSockets; i++) {
        if (_fds[i].fd >= 0) {
            close(_fds[i].fd);
        }
    }
    close(_serverSocket);
}

std::string NetworkServer::getClientIp(int socket_fd)
{
    sockaddr_storage addr;
    socklen_t len = sizeof(addr);

    if (getpeername(socket_fd, (sockaddr *) &addr, &len) == -1) {
        return "unknown";
    }

    char ipStr[INET_ADDRSTRLEN];
    const char *result = nullptr;

    if (addr.ss_family == AF_INET) {
        sockaddr_in *s = (struct sockaddr_in *) &addr;
        result = inet_ntop(AF_INET, &s->sin_addr, ipStr, INET6_ADDRSTRLEN);
    }
    if (result == nullptr) {
        return "unknown";
    }
    return std::string(ipStr);
}

void NetworkServer::run()
{
    while (true) {
        int activity = poll(_fds, _nbSockets, -1);

        if (activity < 0) {
            _logger.log(Logger::LogLevel::ERROR, "failed to poll network activity");
            break;
        }

        if (_fds[0].revents & POLLIN) {
            sockaddr_in clientAddress;
            socklen_t clientAddressLen = sizeof(clientAddress);

            _logger.log(Logger::LogLevel::DEBUG, "incoming client connection");
            // handling too many clients
            if (_nbSockets == MAX_CLIENTS + 1) {
                int tempSocket = accept(_serverSocket,
                                        (struct sockaddr *) &clientAddress,
                                        &clientAddressLen);
                _logger.log(Logger::LogLevel::WARN, "cannot connect new client, reached simultaneous clients limit");
                if (tempSocket != -1) {
                    const char *msg = "server too busy, try again later\r\n";
                    write(tempSocket, msg, strlen(msg));
                    close(tempSocket);
                } else {
                    _logger.log(Logger::LogLevel::ERROR, "failed to send message to incoming client");
                }
            } else {
                // accept new client
                int clientSocket = accept(_serverSocket,
                                          (sockaddr *) &clientAddress,
                                          &clientAddressLen);

                if (clientSocket != -1) {
                    _fds[_nbSockets].fd = clientSocket;
                    _fds[_nbSockets].events = POLLIN;
                    _nbSockets++;
                    _clients.emplace(clientSocket, Client(_logger, clientSocket));
                    _logger.log(Logger::LogLevel::INFO,
                                std::format("client from {} connected on remote port {}",
                                            inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port)));
                } else {
                    _logger.log(Logger::LogLevel::ERROR, "cannot accept incoming client connection");
                }
            }
        }

        // check every client's socket for change and read its data
        for (int i = 1; i < _nbSockets; i++) {
            // i = 1 to skip server's socket
            if (_fds[i].revents & POLLIN) {
                char buffer[4096];
                int bytesRead = read(_fds[i].fd, buffer, sizeof(buffer));

                if (bytesRead <= 0) {
                    _logger.log(Logger::LogLevel::INFO,
                                std::format("client from {} disconnected", getClientIp(_fds[i].fd)));
                    // Client disconnected or error
                    close(_fds[i].fd);
                    _clients.erase(_fds[i].fd);
                    // Remove from poll array by shifting every item after removed one to the left
                    memmove(&_fds[i], &_fds[i + 1],
                            (_nbSockets - i - 1) * sizeof(pollfd));
                    _nbSockets--;
                    i--;
                } else {
                    _clients.at(_fds[i].fd).handleRequest(buffer);
                    memset(buffer, 0, bytesRead);
                }
            }
        }
    }
}
