/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** quitHandler.cpp
*/

#include "quitHandler.hpp"

void quitHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize != 0) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    state->setAuthStatus(false);
    sendResponse(FtpResponse::SERVICE_CLOSING, state);
    close(state->getSocketFd());
}
