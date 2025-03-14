/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** retrHandler.cpp
*/

#include "retrHandler.hpp"

#include "retrHandler.hpp"
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

namespace fs = std::filesystem;

void retrHandler::sendFile(const std::string &filePath, int dataSocket)
{
    int filefd = open(filePath.c_str(), O_RDONLY);
    if (filefd == -1) {
        return;
    }

    struct stat fileStat;
    fstat(filefd, &fileStat);
    off_t offset = 0;
    size_t remaining = fileStat.st_size;

    while (remaining > 0) {
        ssize_t sent = sendfile(dataSocket, filefd, &offset, remaining);
        if (sent <= 0) {
            break;
        }
        remaining -= sent;
    }

    close(filefd);
}

void retrHandler::handleActiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state)
{
    int dataSocket = state->getDataSocket();
    if (dataSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);
    sendFile(filePath, dataSocket);
    close(dataSocket);

    sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
}

void retrHandler::handlePassiveMode(const std::string &filePath, std::shared_ptr<SessionState> &state)
{
    int listeningSocket = state->getDataSocket();
    if (listeningSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);

    sockaddr_in clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int dataSocket = accept(listeningSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (dataSocket < 0) {
        close(listeningSocket);
        state->setDataSocket(-1);
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendFile(filePath, dataSocket);

    close(dataSocket);
    close(listeningSocket);
    state->setDataSocket(-1);

    sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
}

void retrHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
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
    const std::string currentDir = state->getCurrentDirectory();

    if (requestedPath[0] == '/') {
        filePath = state->getRootDirectory() + requestedPath;
    } else {
        if (currentDir == "/") {
            filePath = state->getRootDirectory() + "/" + requestedPath;
        } else {
            filePath = state->getRootDirectory() + currentDir + "/" + requestedPath;
        }
    }

    if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }

    if (access(filePath.c_str(), R_OK) != 0) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE_NOT_FOUND, state);
        return;
    }

    if (mode == SessionState::TransferMode::ACTIVE) {
        handleActiveMode(filePath, state);
    } else if (mode == SessionState::TransferMode::PASSIVE) {
        handlePassiveMode(filePath, state);
    }
}