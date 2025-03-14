/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** listHandler.cpp
*/

#include "listHandler.hpp"
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>

namespace fs = std::filesystem;

void listHandler::executeListCommand(const std::string &path, int dataSocket)
{
    pid_t pid = fork();

    if (pid == -1) {
        return;
    }
    if (pid == 0) {
        dup2(dataSocket, STDOUT_FILENO);

        int devNull = open("/dev/null", O_WRONLY);
        if (devNull != -1) {
            dup2(devNull, STDERR_FILENO);
            close(devNull);
        }
        for (int i = 3; i < 1024; i++) {
            close(i);
        }

        execl("/bin/ls", "ls", "-la", path.c_str(), NULL);
        exit(EXIT_FAILURE);
    }
    int status;
    waitpid(pid, &status, 0);
}

void listHandler::handleActiveMode(const std::string &path, std::shared_ptr<SessionState> &state)
{
    const int dataSocket = state->getDataSocket();
    if (dataSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);
    executeListCommand(path, dataSocket);
    close(dataSocket);
    sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
}

void listHandler::handlePassiveMode(const std::string &path, std::shared_ptr<SessionState> &state)
{
    const int listeningSocket = state->getDataSocket();
    if (listeningSocket < 0) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    sendResponse(FtpResponse::FILE_STATUS_OK_OPENING_CONN, state);

    sockaddr_in clientAddr {};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int dataSocket = accept(listeningSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (dataSocket < 0) {
        close(listeningSocket);
        state->setDataSocket(-1);
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    executeListCommand(path, dataSocket);
    close(dataSocket);
    close(listeningSocket);
    state->setDataSocket(-1);
    sendResponse(FtpResponse::CLOSING_DATA_CONN, state);
}

void listHandler::handleRequest(const std::vector<std::string> &args, std::shared_ptr<SessionState> &state) const
{
    if (!state->isAuthenticated()) {
        sendResponse(FtpResponse::NOT_LOGGED_IN, state);
        return;
    }
    const SessionState::TransferMode mode = state->getTransferMode();
    if (mode == SessionState::TransferMode::NONE) {
        sendResponse(FtpResponse::CANT_OPEN_DATA_CONN, state);
        return;
    }

    std::string directoryPath;
    if (args.empty()) {
        std::string currentDir = state->getCurrentDirectory();
        directoryPath = state->getRootDirectory() + currentDir;
        if (currentDir == "/") {
            directoryPath = state->getRootDirectory();
        }
    } else {
        const std::string& requestedPath = args[0];
        std::string currentDir = state->getCurrentDirectory();

        if (requestedPath[0] == '/') {
            directoryPath = state->getRootDirectory() + requestedPath;
        } else {
            if (currentDir == "/") {
                directoryPath = state->getRootDirectory() + "/" + requestedPath;
            } else {
                directoryPath = state->getRootDirectory() + currentDir + "/" + requestedPath;
            }
        }
    }
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        sendResponse(FtpResponse::FILE_UNAVAILABLE, state);
        return;
    }

    if (mode == SessionState::TransferMode::ACTIVE) {
        handleActiveMode(directoryPath, state);
    } else if (mode == SessionState::TransferMode::PASSIVE) {
        handlePassiveMode(directoryPath, state);
    }
}