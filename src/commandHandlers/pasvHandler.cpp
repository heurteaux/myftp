/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** pasvHandler.cpp
*/

#include "pasvHandler.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <netdb.h>
#include <vector>
#include "sessionState.hpp"

int pasvHandler::createPassiveSocket(std::string &ipAddress, int &port)
{
    int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataSocket < 0) {
        return -1;
    }

    constexpr int opt = 1;
    if (setsockopt(dataSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        close(dataSocket);
        return -1;
    }

    ifaddrs *ifap, *ifa;
    sockaddr_in *sa;
    char host[NI_MAXHOST];

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            sa = reinterpret_cast<sockaddr_in *>(ifa->ifa_addr);
            if ((ifa->ifa_flags & IFF_LOOPBACK) == 0) {
                inet_ntop(AF_INET, &(sa->sin_addr), host, NI_MAXHOST);
                ipAddress = host;
                break;
            }
        }
    }
    freeifaddrs(ifap);

    if (ipAddress.empty()) {
        ipAddress = "127.0.0.1";
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = 0;

    if (bind(dataSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        close(dataSocket);
        return -1;
    }

    if (listen(dataSocket, 5) < 0) {
        close(dataSocket);
        return -1;
    }

    socklen_t addrLen = sizeof(serverAddr);
    if (getsockname(dataSocket, (sockaddr *)&serverAddr, &addrLen) < 0) {
        close(dataSocket);
        return -1;
    }

    port = ntohs(serverAddr.sin_port);
    return dataSocket;
}

void pasvHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    if (!args.empty()) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }

    std::string ipAddress;
    int portNumber;
    const int dataSocket = createPassiveSocket(ipAddress, portNumber);

    if (dataSocket < 0) {
        sendResponse(FtpResponse::DATA_CONN_OPEN_NO_TRANSFER, state);
        return;
    }
    state->setDataSocket(dataSocket);
    state->setTransferMode(SessionState::TransferMode::PASSIVE);
    std::replace(ipAddress.begin(), ipAddress.end(), '.', ',');
    const int p1 = portNumber / 256;
    const int p2 = portNumber % 256;
    const std::string response = "227 Entering Passive Mode (" + ipAddress + "," +
                           std::to_string(p1) + "," + std::to_string(p2) + ").\r\n";
    sendCustomResponse(response, state);
}