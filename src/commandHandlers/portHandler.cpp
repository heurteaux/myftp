/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** portHandler.cpp
*/

#include "portHandler.hpp"
#include <sstream>
#include <arpa/inet.h>
#include <cstring>

bool portHandler::parsePortArgument(const std::string &arg, std::string &ipAddress, int &port)
{
    std::stringstream ss(arg);
    std::string token;
    std::vector<int> values;

    while (std::getline(ss, token, ',')) {
        try {
            int value = std::stoi(token);
            if (value < 0 || value > 255) {
                return false;
            }
            values.push_back(value);
        } catch (const std::exception &) {
            return false;
        }
    }

    if (values.size() != 6) {
        return false;
    }

    ipAddress = std::to_string(values[0]) + "." +
                std::to_string(values[1]) + "." +
                std::to_string(values[2]) + "." +
                std::to_string(values[3]);

    port = values[4] * 256 + values[5];
    return true;
}

void portHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }
    if (args.size() != 1) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    std::string clientIp;
    int clientPort;

    if (!parsePortArgument(args[0], clientIp, clientPort)) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (state->getDataSocket() != -1) {
        close(state->getDataSocket());
    }
    state->setDataIpAddress(clientIp);
    state->setDataPort(clientPort);
    state->setTransferMode(SessionState::TransferMode::ACTIVE);
    sendResponse(FtpResponse::COMMAND_OK, state);
}