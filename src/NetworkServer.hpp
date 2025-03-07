/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** networkServer.hpp
*/

#ifndef NETWORKSERVER_HPP
#define NETWORKSERVER_HPP

#include <netinet/in.h>
#include <vector>
#include <poll.h>
#include <bits/basic_string.h>

#define ERROR_LOG_TITLE "[ERROR]: "
#define UNKNOWN_ERROR_MSG "An unknown error occured on the network stack"
#define CANNOT_INIT_CONN_ERROR "Cannot initialize connection"
#define INVALID_SOCKET_DCONN_ERR "Cannot disconnect from invalid socket"


namespace network
{
    class NetworkServer {

        int _serverSocket;
        int _port;
        sockaddr_in _serverAddress {};
        std::vector<pollfd> _poll_fds;

        void handleNewConnection();

        void handleDisconnect(int fd);

        void handleRequest(int fd);


        public:
            explicit NetworkServer(int port);

            void handleConnections();

            ~NetworkServer();

            static std::string remoteIpStringFromFd(int fd);

            std::string remotePortFromFd(int fd);
    };
}




#endif //NETWORKSERVER_HPP
