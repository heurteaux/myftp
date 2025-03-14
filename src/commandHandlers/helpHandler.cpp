/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** helpHandler.cpp
*/

#include "helpHandler.hpp"

void helpHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize != 0) {
        sendResponse(FtpResponse::SYNTAX_ERROR, state);
        return;
    }
    sendResponse(FtpResponse::HELP_MESSAGE, state);
}