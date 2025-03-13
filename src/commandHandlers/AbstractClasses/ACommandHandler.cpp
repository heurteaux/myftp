/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** aCommandHandler.cpp
*/

#include "ACommandHandler.hpp"

void ACommandHandler::sendResponse(FtpResponse response, const std::shared_ptr<SessionState> &session)
{
    if (!protocolResponses.contains(static_cast<int>(response))) {
        throw std::runtime_error("provided response code is invalid");
    }

    const std::string res = protocolResponses.at(static_cast<int>(response)).getResponse();
    if (write(session->getSocketFd(), res.c_str(), res.size()) == -1) {
        throw std::runtime_error("an error occurred when writing to network socket");
    }
}

void ACommandHandler::sendCustomResponse(const std::string &response, const std::shared_ptr<SessionState> &session)
{
    if (write(session->getSocketFd(), response.c_str(), response.size()) == -1) {
        throw std::runtime_error("an error occurred when writing to network socket");
    }
}