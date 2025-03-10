/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** client.cpp
*/

#include "client.hpp"

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

void Client::sendResponse(const std::string &response) const
{
    write(_socketFd, response, sizeof(msg));
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
    if (command == "USER") {
        _logger.log(Logger::LogLevel::DEBUG, "user it is indeed");
        for (const auto &arg : args) {
            _logger.log(Logger::LogLevel::DEBUG, arg);
        }
    }
    _logger.log(Logger::LogLevel::DEBUG, command);
}