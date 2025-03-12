/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** deleHandler.cpp
*/

#include "deleHandler.hpp"
#include <filesystem>

void deleHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    namespace fs = std::filesystem;

    const size_t aSize = args.size();
    if (aSize > 1 || aSize == 0) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }

    const std::string &newDirectory = args[0];

    const fs::path rootDirectory = state->getRootDirectory();
    const fs::path currentDir = rootDirectory / state->getCurrentDirectory();
    fs::path targetPath;

    if (newDirectory[0] == '/') {
        targetPath = rootDirectory / newDirectory.substr(1);
    } else {
        targetPath = rootDirectory / currentDir.string().substr(1) / newDirectory;
    }

    std::error_code ec;
    fs::path canonicalPath = fs::canonical(targetPath, ec);
    if (ec) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }

    auto relPath = fs::relative(canonicalPath, rootDirectory, ec);
    if (ec || relPath.string().starts_with("..")) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }
    if (!fs::exists(canonicalPath, ec) || ec) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }
    if (access(canonicalPath.parent_path().c_str(), W_OK) != 0) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }
    if (fs::is_directory(canonicalPath, ec) && !ec) {
        if (!fs::remove_all(canonicalPath, ec) || ec) {
            sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
            return;
        }
    } else {
        if (!fs::remove(canonicalPath, ec) || ec) {
            sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
            return;
        }
    }
    sendResponse(FtpResponse::FILE_ACTION_COMPLETED, state);
}
