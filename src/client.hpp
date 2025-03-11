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
#include <unordered_map>
#include <utility>

class Client {
    public:
        explicit Client(Logger &logger, const int socketFd, std::string rootDirectory) : _socketFd(socketFd),
                                                                                         _logger(logger),
                                                                                         _rootDirectory(
                                                                                             std::move(rootDirectory))
        {
            initCommandHandlers();
        }

        ~Client() = default;

        void handleRequest(const std::string &request);

    private:
        using CommandHandler = void (Client::*)(const std::vector<std::string> &);
        using CommandMap = std::unordered_map<std::string, CommandHandler>;
        int _socketFd;
        Logger &_logger;
        CommandMap _commandHandlers;
        bool _isAuthenticated;
        std::string _rootDirectory;
        std::string _currentDirectory = "/";
        std::string _username;

        void sendResponse(const std::string &response) const;

        void initCommandHandlers();

        void handleUSER(const std::vector<std::string> &args);

        void handlePASS(const std::vector<std::string> &args);

        void handleCWD(const std::vector<std::string> &args);

        void handleCDUP(const std::vector<std::string> &args);

        void handleQUIT(const std::vector<std::string> &args);

        void handleDELE(const std::vector<std::string> &args);

        void handlePWD(const std::vector<std::string> &args);

        void handlePASV(const std::vector<std::string> &args);

        void handlePORT(const std::vector<std::string> &args);

        void handleHELP(const std::vector<std::string> &args);

        void handleNOOP(const std::vector<std::string> &args);

        void handleRETR(const std::vector<std::string> &args);

        void handleSTOR(const std::vector<std::string> &args);

        void handleLIST(const std::vector<std::string> &args);
};


#endif //CLIENT_HPP
