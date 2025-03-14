/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** userHandler.cpp
*/

#include "userHandler.hpp"
#include "sessionState.hpp"

void userHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize != 1) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    const std::string &username = args[0];

    state->setUsername(username);
    state->setAuthStatus(true);
    sendResponse(FtpResponse::USERNAME_OK_NEED_PASSWORD, state);
}
