/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** storHandler.cpp
*/

#include "storHandler.hpp"

#include "storHandler.hpp"
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>

namespace fs = std::filesystem;

bool storHandler::receiveFile(const std::string &filePath, int dataSocket) const
{
    const int fileFd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fileFd == -1) {
        return false;
    }

    char buffer[8192];
    ssize_t bytesRead;
    bool success = true;

    while ((bytesRead = read(dataSocket, buffer, sizeof(buffer))) > 0) {
        ssize_t bytesWritten = write(fileFd, buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            success = false;
            break;
        }
    }

    if (bytesRead < 0) {
        success = false;
    }

    close(fileFd);
    return success;
}

void storHandler::handleActiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) const
{
    const int dataSocket = state->getDataSocket();
    if (dataSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);

    bool success = receiveFile(filePath, dataSocket);
    close(dataSocket);

    if (success) {
        sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
    } else {
        unlink(filePath.c_str());
        sendResponse(FtpResponse::ACTION_ABORTED_LOCAL_ERROR, state);
    }
}

void storHandler::handlePassiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state) const
{
    int listeningSocket = state->getDataSocket();
    if (listeningSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);

    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int dataSocket = accept(listeningSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (dataSocket < 0) {
        close(listeningSocket);
        state->setDataSocket(-1);
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    bool success = receiveFile(filePath, dataSocket);

    close(dataSocket);
    close(listeningSocket);
    state->setDataSocket(-1);

    if (success) {
        sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
    } else {
        unlink(filePath.c_str());
        sendResponse(FtpResponse::ACTION_ABORTED_LOCAL_ERROR, state);
    }
}

void storHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }

    if (args.size() != 1) {
        sendResponse(FtpResponse::SYNTAX_ERROR_PARAMS, state);
        return;
    }

    SessionState::TransferMode mode = state->getTransferMode();
    if (mode == SessionState::TransferMode::NONE) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    std::string filePath;
    const std::string& requestedPath = args[0];
    std::string currentDir = state->getCurrentDirectory();

    if (requestedPath[0] == '/') {
        filePath = state->getRootDirectory() + requestedPath;
    } else {
        if (currentDir == "/") {
            filePath = state->getRootDirectory() + "/" + requestedPath;
        } else {
            filePath = state->getRootDirectory() + currentDir + "/" + requestedPath;
        }
    }

    fs::path parentPath = fs::path(filePath).parent_path();
    if (!fs::exists(parentPath) || !fs::is_directory(parentPath)) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }

    if (access(parentPath.c_str(), W_OK) != 0) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }

    if (fs::exists(filePath) && access(filePath.c_str(), W_OK) != 0) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }

    if (mode == SessionState::TransferMode::ACTIVE) {
        handleActiveMode(filePath, state);
    } else if (mode == SessionState::TransferMode::PASSIVE) {
        handlePassiveMode(filePath, state);
    }
}