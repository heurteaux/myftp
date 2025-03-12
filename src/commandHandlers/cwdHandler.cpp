/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** cwdHandler.cpp
*/

#include "cwdHandler.hpp"
#include <filesystem>

void cwdHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    namespace fs = std::filesystem;

    const size_t aSize = args.size();
    if (aSize != 1) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }

    const std::string &newDirectory = args[0];

    if (newDirectory.empty()) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    const fs::path rootDirectory = state->getRootDirectory();
    const fs::path currentDir = rootDirectory / state->getCurrentDirectory();
    fs::path targetPath;
    fs::path newCurrentDir;

    if (newDirectory[0] == '/') {
        newCurrentDir = newDirectory;
        targetPath = rootDirectory / newDirectory.substr(1);
    } else {
        newCurrentDir = currentDir / newDirectory;
        targetPath = rootDirectory / newCurrentDir.string().substr(1);
    }

    std::error_code ec;
    newCurrentDir = fs::weakly_canonical(newCurrentDir, ec);
    fs::path canonicalPath = fs::canonical(targetPath, ec);
    if (ec) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }

    auto relPath = fs::relative(canonicalPath, rootDirectory, ec);
    if (ec || relPath.string().starts_with("..")) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }
    if (access(canonicalPath.c_str(), R_OK) != 0) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }
    if (!fs::is_directory(canonicalPath, ec) || ec) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }

    state->setCurrentDirectory(newCurrentDir.string());
    sendResponse(FtpResponse::FILE_ACTION_COMPLETED, state);
}