/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** passHandler.cpp
*/

#include "passHandler.hpp"
#include <memory>

void passHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize > 1) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (state->getUsername().empty()) {
        sendResponse(FtpResponse::BAD_CMD_SEQUENCE, state);
        return;
    }

    // no true check for the password is performed
    // as there is no user handling, we expect an empty password
    // for the anonymous user
    if (aSize == 0 && state->getUsername() == GUEST_USER) {
        state->setAuthStatus(true);
        sendResponse(FtpResponse::USER_LOGGED_IN, state);
    } else {
        state->setUsername("");
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
    }
}
