/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** client.hpp
*/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "logger.hpp"
#include "unistd.h"

class Client {
    public:
        explicit Client(Logger &logger, const int socketFd) : _socketFd(socketFd), _logger(logger) {}
        ~Client() = default;
        void handleRequest(const std::string& request);

    private:
        int _socketFd;
        Logger &_logger;

        void sendResponse(const std::string &response) const;
};



#endif //CLIENT_HPP
