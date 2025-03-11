/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** client.cpp
*/

#include "client.hpp"
#include <filesystem>

std::string trimAfterCRLF(const std::string& input) {
    size_t pos = input.find("\r\n");

    if (pos != std::string::npos) {
        return input.substr(0, pos);
    }
    throw std::runtime_error("no CRLF found in input");
}

std::vector<std::string> split(const std::string &s) {
    std::istringstream iss(s);
    std::vector<std::string> result;
    std::string token;

    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

void Client::initCommandHandlers() {
    _commandHandlers["USER"] = &Client::handleUSER;
    _commandHandlers["PASS"] = &Client::handlePASS;
    _commandHandlers["CWD"] = &Client::handleCWD;
    _commandHandlers["CDUP"] = &Client::handleCDUP;
    _commandHandlers["QUIT"] = &Client::handleQUIT;
    _commandHandlers["DELE"] = &Client::handleDELE;
    _commandHandlers["PWD"] = &Client::handlePWD;
    //_commandHandlers["PASV"] = &Client::handlePASV;
    //_commandHandlers["PORT"] = &Client::handlePORT;
    //_commandHandlers["HELP"] = &Client::handleHELP;
    _commandHandlers["NOOP"] = &Client::handleNOOP;
    //_commandHandlers["RETR"] = &Client::handleRETR;
    //_commandHandlers["STOR"] = &Client::handleSTOR;
    //_commandHandlers["LIST"] = &Client::handleLIST;
}

void Client::sendResponse(const std::string &response) const
{
    write(_socketFd, response.data(), response.size());
}


void Client::handleRequest(const std::string &request)
{
    // remove anything past first CLRF and catches error about missing CLRF
    try {
        trimAfterCRLF(request);
    } catch (std::runtime_error &e) {
        _logger.log(Logger::LogLevel::ERROR, e.what());
        constexpr char msg[] = "xxx missing CLRF\r\n";
        write(_socketFd, msg, sizeof(msg));
        return;
    }

    // split based on the spaces
    std::vector<std::string> chunkedRequest = split(request);

    if (chunkedRequest.empty()) {
        _logger.log(Logger::LogLevel::ERROR, "empty command");
        constexpr char msg[] = "xxx missing command\r\n";
        write(_socketFd, msg, sizeof(msg));
        return;
    }
    std::vector args(chunkedRequest.begin() + 1, chunkedRequest.end());
    std::string command = chunkedRequest[0];

    // uppercase command
    for (auto &c : command) {
        c = std::toupper(c);
    }

    if (_commandHandlers.contains(command)) {
        _logger.log(Logger::LogLevel::DEBUG, "Executing command: " + command);
        (this->*(_commandHandlers.at(command)))(args);
    } else {
        _logger.log(Logger::LogLevel::ERROR, "Unknown command: " + command);
        sendResponse("xxx Command not implemented\r\n");
    }
}

void Client::handleUSER(const std::vector<std::string> &args) {
    if (args.empty()) {
        sendResponse("501 Syntax error in parameters or arguments\r\n");
        return;
    }
    if (args[0] != "Anonymous") {
        sendResponse("xxx Invalid username\r\n");
        _logger.log(Logger::LogLevel::WARN, "unknown user " + args[0]);
        return;
    }
    _username = args[0];
    _isAuthenticated = false;
    _logger.log(Logger::LogLevel::INFO, "User " + _username + " attempting to login");
    sendResponse("331 User name okay, need password\r\n");
}

void Client::handlePASS(const std::vector<std::string> &args) {
    if (_username.empty()) {
        sendResponse("503 Need account for login\r\n");
        return;
    }
    if (args.empty() || args[0].empty()) {
        _isAuthenticated = true;
        _logger.log(Logger::LogLevel::INFO, "User " + _username + " authenticated");
        sendResponse("230 User logged in, proceed\r\n");
        return;
    }
    sendResponse("xxx Invalid password\r\n");
}

void Client::handleCWD(const std::vector<std::string>& args) {
    if (!_isAuthenticated) {
        sendResponse("530 Not logged in.\r\n");
        return;
    }

    if (args.empty()) {
        sendResponse("501 Syntax error in parameters or arguments.\r\n");
        return;
    }

    std::string fullpath = _rootDirectory + _currentDirectory + args[0];
    _logger.log(Logger::LogLevel::DEBUG, fullpath);
    if (!std::filesystem::exists(fullpath) || !std::filesystem::is_directory(fullpath)) {
        sendResponse("550 Directory not found.\r\n");
        return;
    }
    _currentDirectory = _currentDirectory + args[0] + '/';
    sendResponse("250 Requested file action okay, completed.\r\n");
}

void Client::handleCDUP(const std::vector<std::string> &) {
    if (!_isAuthenticated) {
        sendResponse("530 Not logged in.\r\n");
        return;
    }
    _currentDirectory = "./";
    sendResponse("200 Command okay.\nLogged out if appropriate.\r\n");
}

void Client::handleQUIT(const std::vector<std::string> &) {
    sendResponse("221 Bye !\r\n");
    close(_socketFd);
}

void Client::handleDELE(const std::vector<std::string>& args) {
    if (!_isAuthenticated) {
        sendResponse("530 Not logged in.\r\n");
        return;
    }

    if (args.empty()) {
        sendResponse("501 Syntax error in parameters or arguments.\r\n");
        return;
    }

    std::string filePath = _rootDirectory + args[0];
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
        sendResponse("550 File not found.\r\n");
        return;
    }
    std::filesystem::remove(filePath);
    sendResponse("250 Requested file action okay, completed.\r\n");
}

void Client::handlePWD(const std::vector<std::string> &) {
    if (!_isAuthenticated) {
        sendResponse("530 Not logged in.\r\n");
        return;
    }
    sendResponse(std::format("257 {}\r\n", _currentDirectory));
}

void Client::handleNOOP(const std::vector<std::string> &) {
    _logger.log(Logger::LogLevel::DEBUG, "NOOP command received");
    sendResponse("200 Why ?\r\n");
}