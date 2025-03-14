/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** pwdHandler.cpp
*/

#include "pwdHandler.hpp"

void pwdHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    const size_t aSize = args.size();
    if (aSize != 0) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }

    std::string currentPath = state->getCurrentDirectory();
    if (!currentPath.empty() && currentPath.back() != '/') {
        currentPath += '/';
    }
    const std::string pathResponse = "\"" + currentPath + "\"";
    const std::string response = "257 " + pathResponse + " is current directory.\r\n";
    sendCustomResponse(response, state);
}
