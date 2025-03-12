/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** noopHandler.cpp
*/

#include "noopHandler.hpp"

void noopHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize != 0) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    sendResponse(FtpResponse::COMMAND_OK, state);
}