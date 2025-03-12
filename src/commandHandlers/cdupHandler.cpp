/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** cdupHandler.cpp
*/

#include "cdupHandler.hpp"
#include <filesystem>

void cdupHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
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

    namespace fs = std::filesystem;
    const fs::path currentRelPath = state->getCurrentDirectory();

    // Handling case where we are already at the root directory
    if (currentRelPath.empty() || currentRelPath == "." || currentRelPath == "/") {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }

    const fs::path newRelPath = currentRelPath.parent_path();
    state->setCurrentDirectory(newRelPath.string());
    sendResponse(FtpResponse::COMMAND_OK, state);
}
