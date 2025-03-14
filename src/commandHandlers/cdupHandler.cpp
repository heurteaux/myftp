/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** cdupHandler.cpp
*/

#include "cdupHandler.hpp"
#include <filesystem>
#include "cwdHandler.hpp"

void cdupHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    namespace fs = std::filesystem;
    const size_t aSize = args.size();
    if (aSize != 0) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }

    const std::string &newDirectory = "..";
    if (newDirectory.empty()) {
        sendResponse(FtpResponse::COMMAND_OK, state);
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
    newCurrentDir = newCurrentDir.lexically_normal();
    const fs::path canonicalPath = fs::canonical(targetPath, ec).lexically_normal();
    if (ec) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }

    auto relPath = fs::relative(canonicalPath, rootDirectory, ec);
    if (ec || relPath.string().starts_with("..")) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }
    if (access(canonicalPath.c_str(), R_OK) != 0) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }
    if (!fs::is_directory(canonicalPath, ec) || ec) {
        sendResponse(FtpResponse::COMMAND_OK, state);
        return;
    }
    std::string dirPath = newCurrentDir.string();
    if (!dirPath.empty() && dirPath.back() != '/') {
        dirPath += '/';
    }
    state->setCurrentDirectory(dirPath);
    sendResponse(FtpResponse::COMMAND_OK, state);
}
